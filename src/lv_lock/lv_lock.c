#include "lv_lock.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "Lv_Lock";

static SemaphoreHandle_t lvgl_mutex;

esp_err_t lvgl_lock_init(void)    
{
    lvgl_mutex = xSemaphoreCreateMutex();
    if (lvgl_mutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create LVGL mutex");
    }

    return lvgl_mutex ? ESP_OK : ESP_FAIL;
}

esp_err_t lvgl_lock(void)
{
    if (lvgl_mutex)
        xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
    return lvgl_mutex ? ESP_OK : ESP_FAIL;
}

esp_err_t lvgl_unlock(void)
{
    if (lvgl_mutex)
        xSemaphoreGive(lvgl_mutex);
    return lvgl_mutex ? ESP_OK : ESP_FAIL;
}