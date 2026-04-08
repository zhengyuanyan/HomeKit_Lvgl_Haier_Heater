#include "task.h"
#include "gui_task/gui_task.h"
#include "input_task/input_task.h"
#include "lv_lock/lv_lock.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "Task";

void task_init(void)
{
    esp_err_t err;
    // 初始化输入队列
    err = input_task_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize input task: %d", err);
    }

    err = lvgl_lock_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize LVGL lock: %d", err);
    }

    // GUI 固定到 core 1
    xTaskCreatePinnedToCore(gui_task, "LVGL_GUI", 8192, NULL, 2, NULL, 1);

    // Input 固定到 core 0
    xTaskCreatePinnedToCore(input_task, "INPUT_TASK", 4096, NULL, 3, NULL, 0);
}