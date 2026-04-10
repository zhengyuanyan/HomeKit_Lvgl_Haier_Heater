#include "gui_water_heater.h"
#include "gui_water_heater_cb.h"
#include "esp_log.h"

static const char *TAG = "GUI_WATER_HEATER";

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

/* =========================
 * 页面创建
 * ========================= */

void gui_water_heater_page(lv_obj_t *parent)
{
    if (!parent)
    {
        ESP_LOGE(TAG, "parent is NULL");
        return;
    }
    gui.water_heater.temperature.ui = ui_water_heater_create(parent, gui.water_heater.temperature.temperature);

    if (!gui.water_heater.temperature.ui)
    {
        ESP_LOGE(TAG, "ui_water_heater_create failed");
        return;
    }

    encoder_add_focus_obj_group_event(gui.water_heater.temperature.ui->arc_water_heater, gui_water_heater_cb, &gui.water_heater.temperature);

    gui.water_heater.active = true;

    ESP_LOGI(TAG, "gui_water_heater_page run");
}

/* =========================
 * 页面删除
 * ========================= */
void gui_water_heater_page_delete(void)
{
    if (gui.water_heater.temperature.ui != NULL)
    {
        if (gui.water_heater.temperature.ui->arc_water_heater)
        {
            encoder_remove_obj_group(gui.water_heater.temperature.ui->arc_water_heater);
        }

        ui_water_heater_delete(gui.water_heater.temperature.ui);
        gui.water_heater.temperature.ui = NULL;
        gui.water_heater.active = false;
    }
    else
    {
        ESP_LOGE(TAG, "gui water heater temperature UI is NULL");
        return;
    }
}