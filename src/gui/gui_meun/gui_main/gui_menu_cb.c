#include "gui_menu_cb.h"
#include "gui_menu.h"

void gui_menu_page_knob_event_cb(lv_event_t *e)
{
    gui_menu_t *ui = lv_event_get_user_data(e);
    if (!ui || !ui->knob.ui)
        return;
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        switch (key)
        {
        case LV_KEY_LEFT:
        case LV_KEY_UP:
            ui_segment_knob_prev(ui->knob.ui);
            break;
        case LV_KEY_RIGHT:
        case LV_KEY_DOWN:
            ui_segment_knob_next(ui->knob.ui);
            break;
        case LV_KEY_ENTER:
            ui_segment_knob_enter(ui->knob.ui);
            break;
        case LV_KEY_ESC:

            break;
        default:
            break;
        }
    }
}