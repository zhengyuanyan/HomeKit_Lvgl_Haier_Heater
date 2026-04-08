#ifndef LV_UI_FONT_H
#define LV_UI_FONT_H

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

    LV_FONT_DECLARE(ui_segment_knob_font_20);
    LV_FONT_DECLARE(ui_segment_knob_font_24);
    LV_FONT_DECLARE(lv_font_number_font_30);

#ifdef __cplusplus
}
#endif

#endif /*LV_UI_FONT_H*/