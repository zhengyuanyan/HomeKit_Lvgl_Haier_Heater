#include "hx711_button.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "esp_log.h"

static const char *TAG = "Hx711_Button";

#define PRESS_THRESHOLD 0.5f
#define RELEASE_THRESHOLD 0.2f // 

#define FILTER_CNT 3

#define ROTATE_BLOCK_TIME 120

#define SPIKE_LIMIT 20000

static float lerp_clamp(float value, float inMin, float inMax, float outMin, float outMax)
{
    if (fabsf(inMax - inMin) < 1e-6f)
        return outMin; // 避免除零
    float u = (value - inMin) / (inMax - inMin);
    if (u < 0.0f)
        u = 0.0f;
    if (u > 1.0f)
        u = 1.0f;
    return u * (outMax - outMin) + outMin;
}

static int32_t strain_filter(Hx711_Button *btn, int32_t raw)
{
    btn->filter_buf[btn->filter_index] = raw;

    btn->filter_index++;

    if (btn->filter_index >= STRAIN_FILTER_SIZE)
        btn->filter_index = 0;

    int64_t sum = 0;

    for (int i = 0; i < STRAIN_FILTER_SIZE; i++)
        sum += btn->filter_buf[i];

    return sum / STRAIN_FILTER_SIZE;
}

void hx711_button_init(Hx711_Button *btn)
{
    if (btn == NULL)
        return;

    btn->is_pressed = false;

    btn->filter_cnt = 0;

    btn->idle_value = 0;

    btn->press_delta = 6000;

    btn->last_raw = 0;

    btn->filter_index = 0;

    btn->last_log_ms = 0;

    for (int i = 0; i < STRAIN_FILTER_SIZE; i++)
        btn->filter_buf[i] = 0;
}

void hx711_button_auto_zero(Hx711_Button *btn, int32_t raw)
{
    if (btn == NULL)
        return;

    btn->idle_value = raw;

    for (int i = 0; i < STRAIN_FILTER_SIZE; i++)
        btn->filter_buf[i] = raw;
}

bool hx711_button_update(Hx711_Button *btn, int32_t raw, uint32_t now, uint32_t encoder_last_move)
{

    // 旋转屏蔽
    if (now - encoder_last_move < ROTATE_BLOCK_TIME)
    {
        btn->filter_cnt = 0;
        return false;
    }

    // 突刺过滤
    if (abs(raw - btn->last_raw) > SPIKE_LIMIT)
    {
        btn->last_raw = raw;
        return btn->is_pressed;
    }
    btn->last_raw = raw;

    // 滤波
    raw = strain_filter(btn, raw);

    // 计算压力归一化，自动处理正负 press_delta
    float press_unit;
    if (btn->press_delta > 0)
    {
        press_unit = lerp_clamp((float)raw, (float)btn->idle_value, (float)(btn->idle_value + btn->press_delta), 0.0f, 1.0f);
    }
    else if (btn->press_delta < 0)
    {
        press_unit = lerp_clamp((float)raw, (float)(btn->idle_value + btn->press_delta), (float)btn->idle_value, 1.0f, 0.0f);
    }
    else
    {
        press_unit = 0.0f;
    }
    // 自动零点跟踪 (缓慢漂移补偿)
    // 仅在未按下且压力很小时更新，避免误触发
    if (!btn->is_pressed && press_unit < 0.1f)
    {
        // 使用更平滑的滤波系数
        const float alpha = 0.001f;
        btn->idle_value = (int32_t)((1.0f - alpha) * btn->idle_value + alpha * raw);
    }

    // 按下检测
    if (!btn->is_pressed)
    {
        if (press_unit > PRESS_THRESHOLD)
        {
            btn->filter_cnt++;
            if (btn->filter_cnt > FILTER_CNT)
            {
                btn->is_pressed = true;
                btn->filter_cnt = 0;
#ifdef hx711_input_debug
                ESP_LOGI(TAG, "Button PRESSED (unit %.2f, raw %ld)", press_unit, raw);
#endif
            }
        }
        else
            btn->filter_cnt = 0;
    }
    else
    {
        // 松开检测
        if (press_unit < RELEASE_THRESHOLD)
        {
            btn->filter_cnt++;
            if (btn->filter_cnt > FILTER_CNT)
            {
                btn->is_pressed = false;
                btn->filter_cnt = 0;
#ifdef hx711_input_debug
                ESP_LOGI(TAG, "Button RELEASED (unit %.2f, raw %ld)", press_unit, raw);
#endif
            }
        }
        else
            btn->filter_cnt = 0;
    }

    // 每秒打印状态
    if (now - btn->last_log_ms > 1000)
    {
#ifdef hx711_input_debug
        ESP_LOGI(TAG, "Status: raw=%ld idle=%ld unit=%.2f pressed=%d", raw, btn->idle_value, press_unit, btn->is_pressed);
#endif

        btn->last_log_ms = now;
    }

    return btn->is_pressed;
}