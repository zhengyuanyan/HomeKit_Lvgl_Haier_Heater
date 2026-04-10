#include "gui_digital_tube_clock.h"
#include "gui_digital_tube_clock_cb.h"
#include "esp_log.h"

static const char *TAG = "gui_digital_tube_clock";

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

void gui_digital_tube_clock_page(void)
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    gui.clock.digital_tube.ui = ui_digital_clock_create(scr);

    if (!gui.clock.digital_tube.ui)
    {
        ESP_LOGE(TAG, "Failed to create digital tube clock UI");
        return;
    }

    /* encoder绑定 */
    encoder_add_focus_obj_group_event(gui.clock.digital_tube.ui->cont, gui_digital_tube_clock_cb, &gui.clock.digital_tube);

    gui.clock.digital_tube.active = true;
}

void gui_digital_tube_clock_page_delete(void)
{

    if (gui.clock.digital_tube.ui != NULL)
    {
        if (gui.clock.digital_tube.ui->cont)
        {
            encoder_remove_obj_group(gui.clock.digital_tube.ui->cont);
        }
        ui_digital_clock_delete(gui.clock.digital_tube.ui);
        gui.clock.digital_tube.ui = NULL;
        gui.clock.digital_tube.active = false;
    }
    else
    {
        ESP_LOGW(TAG, "digital tube clock UI is NULL");
        return;
    }
}
