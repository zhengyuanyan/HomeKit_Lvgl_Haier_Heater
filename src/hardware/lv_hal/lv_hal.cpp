#include "lv_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "../../msg/msg_input.h"
#include "../input/touch_input/touch_input.h"

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif

static TFT_eSPI tft = TFT_eSPI();

extern QueueHandle_t input_queue;
// static input_msg_t msg;

static const char *TAG = "Lv_Hal";
static lv_display_t *lvDisplay;
static lv_indev_t *lvTouchIndev;
static lv_indev_t *lvEncoderIndev;
static lv_group_t *lvGroup;

#define TFT_HOR_RES 240
#define TFT_VER_RES 240
#define TFT_ROTATION LV_DISPLAY_ROTATION_0

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// #define BUF_LINES 40 // 每次刷40行（按RAM调）

// static lv_color_t buf1[TFT_HOR_RES * BUF_LINES];
// static lv_color_t buf2[TFT_HOR_RES * BUF_LINES];

static int encoder_sub_id = -1;

static void module_lvgl_init(void)
{
    encoder_sub_id = input_broadcast_subscribe();
}

static uint32_t my_tick(void)
{
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

static void touchpad_read_cb(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = touch_input_update(&touchX, &touchY);
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
    input_msg_t msg;
    int diff = 0;
    data->enc_diff = 0;

    // if (xQueueReceive(input_queue, &msg, 0) == pdTRUE)
    if (input_broadcast_pop(encoder_sub_id, &msg, 0))
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

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);

    tft.pushColors((uint16_t *)px_map, w * h, true);

    tft.endWrite();

    lv_display_flush_ready(disp);
}

void lv_hal_init(void)
{
#ifdef lv_hal_debug
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    ESP_LOGI(TAG, "%s", LVGL_Arduino.c_str());
#endif

#ifdef esp32s3_dial
    touch_input_init();
#endif

    module_lvgl_init();

    lv_init();

    lv_tick_set_cb(my_tick);

    /* Create LVGL display and set the flush function */

#if LV_USE_TFT_ESPI

    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    lvDisplay = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(lvDisplay, TFT_ROTATION);

#else
    tft.init();
    tft.initDMA();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);

    lvDisplay = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(lvDisplay, disp_flush);
    lv_display_set_buffers(lvDisplay, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
    /* Set the touch input function */

#ifdef esp32s3_dial
    lvTouchIndev = lv_indev_create();
    lv_indev_set_type(lvTouchIndev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(lvTouchIndev, touchpad_read_cb);
#endif
    lvEncoderIndev = lv_indev_create();
    lv_indev_set_type(lvEncoderIndev, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(lvEncoderIndev, encoder_read_cb);

    lvGroup = lv_group_create();
    lv_group_set_default(lvGroup);
    lv_indev_set_group(lvEncoderIndev, lvGroup);

#ifdef lv_hal_debug
    ESP_LOGI(TAG, "LV HAL init done");
#endif
}

lv_group_t *get_lv_group(void)
{
    return lvGroup;
}
