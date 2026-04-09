#ifndef GUI_MEUN_CB_H
#define GUI_MEUN_CB_H

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

void gui_main_page_knob_event_cb(lv_event_t *e);


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif /*GUI_MEUN_CB_H*/