#ifndef HX711_BUTTON_H
#define HX711_BUTTON_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C"
{
#endif

#define STRAIN_FILTER_SIZE 5

    typedef struct
    {
        bool is_pressed; // 当前按键状态

        int32_t idle_value;  // 零点基准值 (自动跟踪)
        int32_t press_delta; // 按下阈值变化量 (可正可负)

        int32_t last_raw; // 上次原始读数 (用于突刺检测)

        int32_t filter_buf[STRAIN_FILTER_SIZE]; // 滑动平均滤波缓冲
        uint8_t filter_index;                   // 滤波器索引

        uint8_t filter_cnt; // 防抖计数器

        uint32_t last_log_ms; // 上次日志打印时间

    } Hx711_Button; // 应变片按键结构体

    /**
     * @brief 初始化应变片按键
     *
     * @param btn 按键结构体指针
     */
    void hx711_button_init(Hx711_Button *btn);

    /**
     * @brief 自动零点跟踪
     *
     * @param btn 按键结构体指针
     * @param raw 原始读数
     */
    void hx711_button_auto_zero(Hx711_Button *btn, int32_t raw);

    /**
     * @brief 更新应变片按键状态
     *
     * @param btn 按键结构体指针
     * @param raw 原始读数
     * @param now 当前时间
     * @param encoder_last_move 编码器上次移动时间
     * @return true 按键状态改变
     * @return false 按键状态未改变
     */
    bool hx711_button_update(Hx711_Button *btn, int32_t raw, uint32_t now, uint32_t encoder_last_move);

#ifdef __cplusplus
}
#endif

#endif
