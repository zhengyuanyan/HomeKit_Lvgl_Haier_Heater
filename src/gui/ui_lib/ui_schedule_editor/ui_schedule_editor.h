#ifndef UI_SCHEDULE_EDITOR_H
#define UI_SCHEDULE_EDITOR_H

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
    /* =========================
      schedule 数据结构（独立）
   ========================= */
    typedef void (*ui_schedule_editor_close_cb_t)(void *user_data);
    typedef struct
    {
        uint8_t enabled;

        uint8_t start_hour;
        uint8_t start_min;

        uint8_t end_hour;
        uint8_t end_min;

        uint8_t state; // 当前状态（运行用）

        void (*on_cb)(void);
        void (*off_cb)(void);

    } schedule_t;

    typedef enum
    {
        FOCUS_START_H = 0,
        FOCUS_START_M,
        FOCUS_END_H,
        FOCUS_END_M,
        FOCUS_SWITCH,
        FOCUS_SAVE,
        FOCUS_MAX
    } schedule_focus_t;

    /* =========================
   MODE
========================= */
    typedef enum
    {
        MODE_FOCUS = 0,
        MODE_EDIT
    } edit_mode_t;

    /* =========================
       UI editor
    ========================= */
    typedef struct
    {
        lv_obj_t *root;

        lv_obj_t *roller_start_h;
        lv_obj_t *roller_start_m;
        lv_obj_t *roller_end_h;
        lv_obj_t *roller_end_m;

        lv_obj_t *sw_enable;
        lv_obj_t *btn_save;

        schedule_focus_t focus;
        edit_mode_t mode;
        int edit_target;
        
        schedule_t *schedule; // ⭐ 指向外部数据

        /* 保存后回调 */
        void (*on_saved)(void *user_data);
        void *user_data;
    } ui_schedule_editor_t;

    /* =========================
       API
    ========================= */
    ui_schedule_editor_t *ui_schedule_editor_create(lv_obj_t *parent, schedule_t *schedule, void (*on_saved)(void *user_data), void *user_data);

    void ui_schedule_editor_delete(ui_schedule_editor_t *ed);

    void ui_schedule_editor_left(ui_schedule_editor_t *ui);  // Encoder 左
    void ui_schedule_editor_right(ui_schedule_editor_t *ui); // Encoder 右
    void ui_schedule_editor_enter(ui_schedule_editor_t *ui); // Encoder 按下
#ifdef __cplusplus
}
#endif

#endif
