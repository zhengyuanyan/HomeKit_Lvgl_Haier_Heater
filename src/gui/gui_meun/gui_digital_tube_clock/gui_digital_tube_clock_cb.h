#ifndef GUI_DIGITAL_TUBE_CLOCK_CB_H
#define GUI_DIGITAL_TUBE_CLOCK_CB_H

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

    void gui_digital_tube_clock_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /*GUI_DIGITAL_TUBE_CLOCK_CB_H */
