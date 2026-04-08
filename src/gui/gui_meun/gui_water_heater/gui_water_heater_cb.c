#include "gui_water_heater_cb.h"
#include "../gui_main/gui_menu.h"
#include "esp_log.h"

static const char *TAG = "gui_water_heater_cb";

void gui_water_heater_cb(lv_event_t *e)
{

    gui_water_heater_t *wh = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint8_t raw_val = ui_water_heater_get_value(wh->temperature.ui);
        uint8_t percent = (uint8_t)LV_CLAMP(35, raw_val, 70);

        gui_water_heater.temperature.value = percent;
    }

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        switch (key)
        {
        case LV_KEY_ESC:
            gui_menu_page();
            break;
        case LV_KEY_ENTER:

            if (!gui_water_heater.temperature.ui)
            {
                ESP_LOGE(TAG, "gui_water_heater.temperature.ui is NULL");
            }
            else
            {
                encoder_add_focus_obj_group_editing(gui_water_heater.temperature.ui->arc_water_heater, true);
            }

            break;
        default:
            break;
        }
    }
}
