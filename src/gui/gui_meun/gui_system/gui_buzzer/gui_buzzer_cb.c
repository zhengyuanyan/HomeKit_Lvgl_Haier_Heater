#include "gui_buzzer_cb.h"
#include "../gui_system/gui_system.h"
#include "esp_log.h"

static const char *TAG = "gui_buzzer_cb";

void gui_system_buzzer_switch_cb(lv_event_t *e)
{
    gui_switch_t *sw = lv_event_get_user_data(e);

    if (!sw || !sw->ui)
    {
        ESP_LOGE(TAG, "sw or sw->ui is NULL");
        return;
    }

    uint32_t key = lv_event_get_key(e);

    switch (key)
    {
    case LV_KEY_LEFT:
    case LV_KEY_UP:
        ui_capsule_switch_left(sw->ui);
        break;

    case LV_KEY_RIGHT:
    case LV_KEY_DOWN:
        ui_capsule_switch_right(sw->ui);
        break;

    case LV_KEY_ENTER:
        ui_capsule_switch_enter(sw->ui);

        sw->value = ui_capsule_switch_get(sw->ui);

        break;

    case LV_KEY_ESC:
        gui_system_page_async();
        break;

    default:
        break;
    }
}