#include "gui_system_cb.h"
#include "gui_system.h"
#include "../../gui_main/gui_main.h"
#include "esp_log.h"

static const char *TAG = "gui_system_cb";

void gui_system_page_knob_event_cb(lv_event_t *e)
{
    ui_segment_knob_t *ui = lv_event_get_user_data(e);
    if (!ui)
    {
        ESP_LOGE(TAG, "ui is NULL");
        return;
    }
       
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        switch (key)
        {
        case LV_KEY_LEFT:
        case LV_KEY_UP:
            ui_segment_knob_prev(ui);
            break;
        case LV_KEY_RIGHT:
        case LV_KEY_DOWN:
            ui_segment_knob_next(ui);
            break;
        case LV_KEY_ENTER:
            ui_segment_knob_enter(ui);
            break;
        case LV_KEY_ESC:
            gui_main_page();
            break;
        default:
            break;
        }
    }
}