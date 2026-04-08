#ifndef GUI_WATER_HEATER_H
#define GUI_WATER_HEATER_H

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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../gui.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void gui_water_heater_page(lv_obj_t *parent);
    void gui_water_heater_page_delete(void);

#ifdef __cplusplus
}
#endif

#endif /*GUI_WATER_HEATER_H*/
