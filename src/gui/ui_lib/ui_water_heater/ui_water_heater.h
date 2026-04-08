#ifndef UI_WATER_HEATER_H
#define UI_WATER_HEATER_H

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
    lv_obj_t *arc_water_heater; //水加热滑块
    lv_obj_t *label_water_heater; //水加热标签

    uint8_t water_heater_value; // 当前水加热值
}ui_water_heater_t;
// 创建水加热控件
ui_water_heater_t *ui_water_heater_create(lv_obj_t *parent, uint8_t water_heater_k);

// 删除水加热控件
void ui_water_heater_delete(ui_water_heater_t *ui);

// 设置水加热值
void ui_water_heater_set_value(ui_water_heater_t *ui, uint8_t water_heater_k);

// 获取水加热值
uint8_t ui_water_heater_get_value(ui_water_heater_t *ui);

#ifdef __cplusplus
}
#endif

#endif /*UI_WATER_HEATER_H*/