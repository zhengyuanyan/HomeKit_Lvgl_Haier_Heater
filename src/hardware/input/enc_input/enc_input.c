#include "enc_input.h"
#include <stdint.h>
#include "driver/gpio.h"
#include "../../encoder/encoder.h"
#include "esp_log.h"

static const char *TAG = "Enc_Input";

#ifndef encoder_button_enter_Pin
#define encoder_button_enter_Pin NULL
#endif

rotary_encoder_t *encoder = NULL;
static uint32_t pcnt_unit = 0;
static int last_steps = 0;
static int acc = 0; // 累计器

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

void encoder_init(void)
{
    rotary_encoder_config_t config;
#if (esp32s3_dial)

    encoder_button_enter(encoder_button_enter_Pin, true);
    config = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t)pcnt_unit, GPIO_NUM_41, GPIO_NUM_40);

#elif (esp32s3_86_box)
    {
        config = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t)pcnt_unit, EC35_A_Pin, EC35_B_Pin);
    }
#endif

    ESP_ERROR_CHECK(rotary_encoder_new_ec35(&config, &encoder));

    ESP_ERROR_CHECK(encoder->set_glitch_filter(encoder, 1));

    ESP_ERROR_CHECK(encoder->start(encoder));
}

int encoder_read(void)
{
    if (encoder)
    {

        int steps = encoder->get_counter_value(encoder);
        int diff = steps - last_steps;
        last_steps = steps;

        acc += diff;

        int out = 0;

        if (acc >= 4)
        {
            out = acc / 4;
            acc %= 4;
        }
        else if (acc <= -4)
        {
            out = acc / 4;
            acc %= 4;
        }

#ifndef enc_input_debug
        {
            if (out != 0)
            {
                ESP_LOGI(TAG, "Encoder step: %d", out);
            }
        }
#endif
        return out;
    }

#ifndef enc_input_debug
    {
        ESP_LOGE(TAG, "Encoder not initialized");
    }
#endif
    return 0;
}
