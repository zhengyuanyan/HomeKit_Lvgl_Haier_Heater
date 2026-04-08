#include "gui_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hardware/hardware_api.h"
#include "../../gui/gui.h"
#include "esp_log.h"
#include "../../lv_lock/lv_lock.h"

static const char *TAG = "Gui_Task";



void gui_task(void *pvParameters)
{
    lv_hal_init();
    gui_run();

    while (1)
    {

        lvgl_lock();     
        uint32_t delay = lv_timer_handler();
        lvgl_unlock();

        if (delay < 1)
            delay = 1;
        if (delay > 500)
            delay = 500;

        vTaskDelay(pdMS_TO_TICKS(delay));
    }
}