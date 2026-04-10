// #include "input_task.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "../../msg/msg_input.h"
// #include "../../hardware/hardware_api.h"
// #include "../../lv_lock/lv_lock.h"

// #include "esp_log.h"

// static const char *TAG = "Input_Task";

// extern QueueHandle_t input_queue;

// static int encoder_last_steps = 0;

// static uint32_t touch_start = 0;
// static bool touch_last = false;
// static uint16_t long_press_ms = 800;
// static uint8_t short_press_ms = 30;

// esp_err_t input_task_init(void)
// {
//     input_queue = xQueueCreate(30, sizeof(input_msg_t));

//     if (input_queue == NULL)
//     {
// #ifdef input_task_debug
//         ESP_LOGE(TAG, "Failed to create input queue");
// #endif
//         return ESP_FAIL;
//     }

//     encoder_button_init();

//     encoder_init();

//     return ESP_OK;
// }

// void input_task(void *pvParameters)
// {
//     input_msg_t msg;

//     while (1)
//     {
//         uint16_t touch_x, touch_y;
//         bool touch = touch_update(&touch_x, &touch_y);
//         int diff = encoder_read();
//         uint8_t key = encoder_button_read();
//         uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

//         if (touch)
//         {
//             uint32_t touch_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
//             if (touch_time - touch_start > long_press_ms)
//             {
//                 msg.type = INPUT_TYPE_TOUCH;
//                 msg.action = INPUT_ACTION_LONG;
//                 msg.data.touch.x = touch_x;
//                 msg.data.touch.y = touch_y;
//                 msg.timestamp = now;

//                 xQueueSend(input_queue, &msg, 0);
// #ifdef input_task_debug
//                 ESP_LOGI(TAG, "Touch: %d %d", msg.data.touch.x, msg.data.touch.y);
// #endif
//             }
//             else
//             {
//                 msg.type = INPUT_TYPE_TOUCH;
//                 msg.action = INPUT_ACTION_SHORT;
//                 msg.data.touch.x = touch_x;
//                 msg.data.touch.y = touch_y;
//                 msg.timestamp = now;

//                 xQueueSend(input_queue, &msg, 0);
// #ifdef input_task_debug
//                 ESP_LOGI(TAG, "Touch: %d %d", msg.data.touch.x, msg.data.touch.y);
// #endif
//             }
//         }

//         if (diff != 0)
//         {
//             msg.type = INPUT_TYPE_ENCODER;
//             msg.action = INPUT_ACTION_ROTATE;
//             msg.data.encoder.diff_value = diff;
//             msg.timestamp = now;

//             xQueueSend(input_queue, &msg, 0);
// #ifdef input_task_debug
//             ESP_LOGI(TAG, "Encoder: %d", msg.data.encoder.diff_value);
// #endif
//         }

//         if (key == LV_KEY_ENTER)
//         {
//             msg.type = INPUT_TYPE_ENCODER;
//             msg.action = INPUT_ACTION_SHORT;
//             msg.data.encoder.key = key;
//             msg.timestamp = now;

//             xQueueSend(input_queue, &msg, 0);
// #ifdef input_task_debug
//             ESP_LOGI(TAG, "Button: %d", msg.data.encoder.key);
// #endif
//         }
//         else if (key == LV_KEY_ESC)
//         {
//             msg.type = INPUT_TYPE_ENCODER;
//             msg.action = INPUT_ACTION_LONG;
//             msg.data.encoder.key = key;
//             msg.timestamp = now;

//             xQueueSend(input_queue, &msg, 0);
// #ifdef input_task_debug
//             ESP_LOGI(TAG, "Button: %d", msg.data.encoder.key);
// #endif
//         }

//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }

#include "input_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "../../msg/msg_input.h"
#include "../../hardware/hardware_api.h"
#include "../../lv_lock/lv_lock.h"

#include "esp_log.h"

static const char *TAG = "Input_Task";

/* ================= 参数配置 ================= */
static uint32_t long_press_ms = 800;
static uint32_t short_press_ms = 30;


/* ================= 初始化 ================= */
esp_err_t input_task_init(void)
{
    encoder_button_init();

    encoder_init();

    input_broadcast_init();

    return ESP_OK;
}

/* ================= 主任务 ================= */
void input_task(void *pvParameters)
{
    input_msg_t msg;

    while (1)
    {
        int diff = encoder_read();
        uint8_t key = encoder_button_read();

        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;


        /* =========================================================
         * ENCODER 旋转
         * ========================================================= */
        if (diff != 0)
        {
            msg.type = INPUT_TYPE_ENCODER;
            msg.action = INPUT_ACTION_ROTATE;
            msg.data.encoder.diff_value = diff;
            msg.timestamp = now;

            input_broadcast_send(msg);

#ifdef input_task_debug
            ESP_LOGI(TAG, "Encoder: %d", diff);
#endif
        }

        /* =========================================================
         * 按键（边沿检测）
         * ========================================================= */
        if (key == LV_KEY_ENTER)
        {
            msg.type = INPUT_TYPE_ENCODER;
            msg.action = INPUT_ACTION_SHORT;
            msg.data.encoder.key = key;
            msg.timestamp = now;

            input_broadcast_send(msg);
#ifdef input_task_debug
            ESP_LOGI(TAG, "Button: %d", msg.data.encoder.key);
#endif
        }
        else if (key == LV_KEY_ESC)
        {
            msg.type = INPUT_TYPE_ENCODER;
            msg.action = INPUT_ACTION_LONG;
            msg.data.encoder.key = key;
            msg.timestamp = now;

            input_broadcast_send(msg);
#ifdef input_task_debug
            ESP_LOGI(TAG, "Button: %d", msg.data.encoder.key);
#endif
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
