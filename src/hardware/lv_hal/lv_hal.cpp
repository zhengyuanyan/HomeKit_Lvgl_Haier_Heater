#include "lv_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "../../msg/msg_input.h"

#if LGFX_86_Box
#include "HX711.h"
#include "../displays/LGFX_86_Box.hpp"
#elif LGFX_M5_Dial
#include "../displays/LGFX_M5_Dial.hpp"
#endif

LGFX tft;

extern QueueHandle_t input_queue;
static input_msg_t msg;

static const char *TAG = "Lv_Hal";
static lv_display_t *lvDisplay;
static lv_indev_t *lvTouchIndev;
static lv_indev_t *lvEncoderIndev;
static lv_group_t *lvGroup;

static const uint32_t screenWidth = WIDTH;
static const uint32_t screenHeight = HEIGHT;

const unsigned int lvBufferSize = screenWidth * 30;
uint8_t lvBuffer[2][lvBufferSize];

static uint32_t my_tick(void)
{
    // 使用 FreeRTOS tick 转换成毫秒
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

static void touchpad_read_cb(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

static void encoder_read_cb(lv_indev_t *drv, lv_indev_data_t *data)
{
    int diff = 0;
    while (xQueueReceive(input_queue, &msg, 0) == pdTRUE)
    {

        if (msg.type == INPUT_TYPE_ENCODER)
        {
            switch (msg.action)
            {
            case INPUT_ACTION_ROTATE:
                diff += msg.data.encoder.diff_value;
                break;

            case INPUT_ACTION_SHORT:
            case INPUT_ACTION_LONG:
                lv_group_send_data(lv_group_get_default(), msg.data.encoder.key);
                break;
            default:
                break;
            }
        }
    }
    data->enc_diff = -diff;
}

static void disp_flush(lv_display_t *display, const lv_area_t *area, unsigned char *data)
{

    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    lv_draw_sw_rgb565_swap(data, w * h);

    if (tft.getStartCount() == 0)
    {
        tft.endWrite();
    }
    tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)data);
    lv_display_flush_ready(display); /* tell lvgl that flushing is done */
}

void lv_hal_init(void)
{
#ifdef lv_hal_debug
    ESP_LOGI(TAG, "LV HAL init");
#endif

    lv_init();

    tft.init();
    tft.initDMA();
    tft.startWrite();

    tft.fillScreen(TFT_BLACK);

    lv_tick_set_cb(my_tick);

    /* Create LVGL display and set the flush function */
    lvDisplay = lv_display_create(screenWidth, screenHeight);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, disp_flush);
    lv_display_set_buffers(lvDisplay, lvBuffer[0], lvBuffer[1], lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Set the touch input function */
    lvTouchIndev = lv_indev_create();
    lv_indev_set_type(lvTouchIndev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(lvTouchIndev, touchpad_read_cb);

    lvEncoderIndev = lv_indev_create();
    lv_indev_set_type(lvEncoderIndev, LV_INDEV_TYPE_ENCODER);
    // lv_indev_set_long_press_time(lvEncoderIndev, LONG_PRESS_MS);
    lv_indev_set_read_cb(lvEncoderIndev, encoder_read_cb);

    lvGroup = lv_group_create();
    lv_group_set_default(lvGroup);
    lv_indev_set_group(lvEncoderIndev, lvGroup);

#ifdef lv_hal_debug
    ESP_LOGI(TAG, "LV HAL init done");
#endif
}

void tft_set_brightness(uint8_t brightness)
{
    tft.setBrightness(brightness);
}

lv_group_t *get_lv_group(void)
{
    return lvGroup;
}
