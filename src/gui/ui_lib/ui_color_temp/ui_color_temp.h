#ifndef UI_COLOR_TEMP_H
#define UI_COLOR_TEMP_H

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
extern "C"
{
#endif

    typedef struct
    {
        lv_obj_t *cont;             // 容器
        lv_obj_t *arc_color_temp;   // 色温滑块
        lv_obj_t *label_color_temp; // 色温标签

        uint32_t color_temp_value; // 当前色温值

        bool is_init; // 是否已经初始化
    } ui_color_temp_t;

    // 创建色温控件
    ui_color_temp_t *ui_color_temp_create(lv_obj_t *parent, uint32_t temp_k);

    // 删除色温控件
    void ui_color_temp_delete(ui_color_temp_t *ui);

    // 设置色温值
    void ui_color_temp_set_value(ui_color_temp_t *ui, uint32_t temp_k);

    // 获取色温值
    uint32_t ui_color_temp_get_value(ui_color_temp_t *ui);

#ifdef __cplusplus
}
#endif

#endif /*UI_COLOR_TEMP_H*/
