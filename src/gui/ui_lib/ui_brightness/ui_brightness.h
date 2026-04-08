#ifndef UI_BRIGHTNESS_H
#define UI_BRIGHTNESS_H

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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    lv_obj_t *cont; //容器
    lv_obj_t *arc_brightness; //亮度滑块
    lv_obj_t *label_brightness; //亮度标签

    uint8_t brightness_value; // 当前亮度值
}ui_brightness_t;

// 创建亮度控件
ui_brightness_t *ui_brightness_create(lv_obj_t *parent, uint8_t brightness_k);

// 删除亮度控件
void ui_brightness_delete(ui_brightness_t *ui);

// 设置亮度值
void ui_brightness_set_value(ui_brightness_t *ui, uint8_t brightness_k);

// 获取亮度值
uint8_t ui_brightness_get_value(ui_brightness_t *ui);

#ifdef __cplusplus
}
#endif

#endif /*UI_BRIGHTNESS_H*/