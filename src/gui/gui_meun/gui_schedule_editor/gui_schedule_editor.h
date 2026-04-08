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

#define SCHEDULE_MAX 4

    /* =========================
     * 数据模型
     * ========================= */
    typedef struct
    {
        uint8_t start_hour;
        uint8_t start_min;
        uint8_t end_hour;
        uint8_t end_min;
        bool enabled;
    } schedule_t;

    /* =========================
     * 数据管理器
     * ========================= */
    typedef struct
    {
        schedule_t items[SCHEDULE_MAX];
        bool inited;
        bool dirty;
    } schedule_mgr_t;

    /* =========================
     * UI状态机
     * ========================= */
    typedef enum
    {
        UI_STATE_IDLE = 0,
        UI_STATE_LIST,
        UI_STATE_EDIT
    } gui_schedule_ui_state_t;

    /* =========================
     * UI句柄（opaque）
     * ========================= */
    typedef struct gui_schedule_editor_s gui_schedule_editor_t;

    /* =========================
     * 数据 API
     * ========================= */
    schedule_mgr_t *schedule_mgr_get(void);
    schedule_t *schedule_get(uint8_t id);

    void schedule_set_dirty(void);

    /* =========================
     * UI API
     * ========================= */
    void gui_schedule_editor_page(void);
    void gui_schedule_editor_page_delete(void);
    void gui_schedule_editor_refresh(void);

#ifdef __cplusplus
}
#endif

#endif /*GUI_SCHEDULE_EDITOR_H*/
