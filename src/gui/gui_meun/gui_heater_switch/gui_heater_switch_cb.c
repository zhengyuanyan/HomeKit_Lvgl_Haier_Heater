#include "gui_heater_switch_cb.h"
#include "../gui_main/gui_main.h"
#include "esp_log.h"

static const char *TAG = "gui_heater_switch_cb";

void gui_heater_switch_cb(lv_event_t *e)
{
    gui_heater_t *sw = lv_event_get_user_data(e);

    if (!sw || !sw->heater.ui)
    {
        ESP_LOGW(TAG, "sw or sw->heater.ui is NULL");
        return;
    }

    uint32_t key = lv_event_get_key(e);

    switch (key)
    {
    case LV_KEY_LEFT:
    case LV_KEY_UP:
        ui_capsule_switch_left(sw->heater.ui);
        break;

    case LV_KEY_RIGHT:
    case LV_KEY_DOWN:
        ui_capsule_switch_right(sw->heater.ui);
        break;

    case LV_KEY_ENTER:
        ui_capsule_switch_enter(sw->heater.ui);

        sw->heater.value = ui_capsule_switch_get(sw->heater.ui);

        break;

    case LV_KEY_ESC:
        gui_main_page();
        break;
    default:
        break;
    }
}