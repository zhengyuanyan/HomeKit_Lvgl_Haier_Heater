#ifndef UI_ALARM_EDITOR_H
#define UI_ALARM_EDITOR_H

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

    typedef void (*alarm_done_cb_t)(int hour, int min, void *user);

    /* 状态 */
    typedef enum
    {
        ALARM_EDIT_HOUR = 0,
        ALARM_EDIT_MIN,
        ALARM_EDIT_CONFIRM
    } alarm_edit_state_t;

    /* UI对象 */
    typedef struct ui_alarm_editor ui_alarm_editor_t;

    /* API */
    ui_alarm_editor_t *ui_alarm_editor_create(lv_obj_t *parent, int hour, int min, alarm_done_cb_t cb, void *user);

    void ui_alarm_editor_delete(ui_alarm_editor_t *ui);

    /* 输入接口 */
    void ui_alarm_editor_encoder(ui_alarm_editor_t *ui, int diff);
    void ui_alarm_editor_click(ui_alarm_editor_t *ui);
#ifdef __cplusplus
}
#endif

#endif