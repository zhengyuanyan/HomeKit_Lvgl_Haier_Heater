#ifndef INPUT_TASK_H
#define INPUT_TASK_H

#ifdef __has_include
#if __has_include("lvgl.h")
#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#define LV_LVGL_H_INCLUDE_SIMPLE
#endif
#endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief 初始化输入任务
     *
     * @return esp_err_t ESP_OK表示成功，ESP_FAIL表示失败
     */
    esp_err_t input_task_init(void);
    void input_task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* INPUT_TASK_H */
