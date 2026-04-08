#ifndef GUI_WATER_HEATER_CB_H
#define GUI_WATER_HEATER_CB_H

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

#ifdef __cplusplus
extern "C"
{
#endif

    void gui_water_heater_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /*GUI_WATER_HEATER_CB_H*/
