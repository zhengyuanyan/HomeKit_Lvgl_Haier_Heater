#include "enc_button_input.h"
#include <stdint.h>
#include "driver/gpio.h"
#include "HX711.h"
#include "../../hx711_button/hx711_button.h"
#include "esp_log.h"

static const char *TAG = "Enc_Button_Input";

HX711 scale;
Hx711_Button knob_btn;

// ================== 配置 ==================
#define LONG_PRESS_MS 800
#define SHORT_PRESS_DEBOUNCE_MS 30
#define ROTATE_SUPPRESS_MS 50 // 旋转时屏蔽按键

// ================== 状态 ==================
static uint32_t press_time = 0;
static uint32_t encoder_last_move = 0;

static bool last_pressed = false;
static bool event_sent = false;
static bool long_sent = false;

// ================== GPIO ==================
static void encoder_button_enter(gpio_num_t gpio, bool active_low)
{
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = active_low ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
        .pull_down_en = active_low ? GPIO_PULLDOWN_DISABLE : GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE};
    gpio_config(&cfg);
}

// ================== 初始化 ==================
void encoder_button_init(void)
{
#if LGFX_M5_Dial
    encoder_button_enter(encoder_button_enter_Pin, true);

#elif LGFX_86_Box
    scale.begin(HX711_PIN_DO, HX711_PIN_SCK);

    hx711_button_init(&knob_btn);

    while (!scale.wait_ready_timeout(100))
        ;

    int32_t raw = scale.read();
    strain_button_auto_zero(&knob_btn, raw);

    ESP_LOGI(TAG, "HX711 Button 初始化完成");
#endif
}

// ================== 主读取函数 ==================
uint8_t encoder_button_read(void)
{
    uint32_t now = millis();
    bool pressed = false;

#if LGFX_M5_Dial
    pressed = !gpio_get_level(encoder_button_enter_Pin);

#elif LGFX_86_Box

    // 👉 非阻塞读取 HX711
    if (scale.is_ready())
    {
        int32_t raw = scale.read();
        hx711_button_update(&knob_btn, raw, now, encoder_last_move);
    }

    pressed = knob_btn.is_pressed;

    // 👉 旋转期间屏蔽按键
    if ((now - encoder_last_move) < ROTATE_SUPPRESS_MS)
    {
        pressed = false;
    }
#endif

    uint8_t key = 0;

    // ================== 按下瞬间 ==================
    if (pressed && !last_pressed)
    {
        press_time = now;
        event_sent = false;
        long_sent = false;

#ifdef enc_button_input_debug
        ESP_LOGI(TAG, "[BTN] PRESSED");
#endif
    }

    // ================== 长按（立即触发） ==================
    if (pressed && !long_sent && !event_sent)
    {
        if ((now - press_time) > LONG_PRESS_MS)
        {
            key = LV_KEY_ESC;
            long_sent = true;
            event_sent = true;

#ifdef enc_button_input_debug
            ESP_LOGI(TAG, "[BTN] LONG PRESS -> ESC");
#endif
        }
    }

    // ================== 松开瞬间 ==================
    if (!pressed && last_pressed && !event_sent)
    {
        uint32_t press_len = now - press_time;

        if (press_len > SHORT_PRESS_DEBOUNCE_MS)
        {
            key = LV_KEY_ENTER;
            event_sent = true;

#ifdef enc_button_input_debug
            ESP_LOGI(TAG, "[BTN] SHORT PRESS -> ENTER (%lu ms)", press_len);
#endif
        }
    }

    last_pressed = pressed;
    return key;
}