#ifndef GUI_SCHEDULE_EDITOR_H
#define GUI_SCHEDULE_EDITOR_H

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
#include <stdbool.h>
#include "../../gui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    void gui_schedule_editor_page(lv_obj_t *parent);

    void gui_schedule_editor_page_delete(void);


#ifdef __cplusplus
}
#endif

#endif /*GUI_SCHEDULE_EDITOR_H*/
