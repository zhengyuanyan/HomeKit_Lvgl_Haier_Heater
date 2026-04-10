#include "gui_digital_tube_clock_cb.h"
#include "../gui_main/gui_main.h"
#include "esp_log.h"

static const char *TAG = "gui_digital_tube_clock_cb";

void gui_digital_tube_clock_cb(lv_event_t *e)
{
    gui_clock_t *cl = lv_event_get_user_data(e);

    if (!cl || !cl->digital_tube.ui)
    {
        ESP_LOGW(TAG, "cl or cl->digital_tube.ui is NULL");
        return;
    }

    uint32_t key = lv_event_get_key(e);

    switch (key)
    {
    case LV_KEY_LEFT:
    case LV_KEY_UP:

        break;

    case LV_KEY_RIGHT:
    case LV_KEY_DOWN:

        break;

    case LV_KEY_ENTER:

        gui_main_page();
        break;

    case LV_KEY_ESC:
        // gui_main_page();
        break;
    default:
        break;
    }
}