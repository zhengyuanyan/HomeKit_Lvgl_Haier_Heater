#include "gui_schedule_editor_cb.h"
#include "gui_schedule_editor.h"
#include "../gui_main/gui_main.h"
#include "esp_log.h"

void gui_schedule_editor_page_knob_event_cb(lv_event_t *e)
{
    gui_knob_t *knob = lv_event_get_user_data(e);
    if (!knob || !knob->ui)
        return;
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        switch (key)
        {
        case LV_KEY_LEFT:
        case LV_KEY_UP:
            ui_segment_knob_prev(knob->ui);
            break;
        case LV_KEY_RIGHT:
        case LV_KEY_DOWN:
            ui_segment_knob_next(knob->ui);
            break;
        case LV_KEY_ENTER:
            ui_segment_knob_enter(knob->ui);
            break;
        case LV_KEY_ESC:
            gui_main_page();
            break;
        default:
            break;
        }
    }
}

void gui_schedule_open_editor_page_event_cb(lv_event_t *e)
{
    gui_schedule_t *schedule = lv_event_get_user_data(e);
    if (!schedule || !schedule->ui)
        return;
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        switch (key)
        {
        case LV_KEY_LEFT:
        case LV_KEY_UP:
            ui_schedule_editor_left(schedule->ui);
            break;
        case LV_KEY_RIGHT:
        case LV_KEY_DOWN:
            ui_schedule_editor_right(schedule->ui);
            break;
        case LV_KEY_ENTER:
            ui_schedule_editor_enter(schedule->ui);
            break;
        case LV_KEY_ESC:

            break;
        default:
            break;
        }
    }
}