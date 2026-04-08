#include "gui_water_heater.h"
#include "gui_water_heater_cb.h"
#include "esp_log.h"

static const char *TAG = "GUI_WATER_HEATER";

/* =========================
 * 全局对象（组件化）
 * ========================= */
gui_water_heater_t gui_water_heater = {
    .temperature = {
        .ui = NULL,
        .value = 35,
    },
};

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
    gui_water_heater.temperature.ui = ui_water_heater_create(parent, gui_water_heater.temperature.value);

    if (!gui_water_heater.temperature.ui)
    {
        ESP_LOGE(TAG, "ui_water_heater_create failed");
        return;
    }

    encoder_add_focus_obj_group_event(gui_water_heater.temperature.ui->arc_water_heater, gui_water_heater_cb, &gui_water_heater.temperature);
}

/* =========================
 * 页面删除
 * ========================= */
void gui_water_heater_page_delete(void)
{
    if (!gui_water_heater.temperature.ui)
    {
        ESP_LOGE(TAG, "gui_water_heater.temperature.ui is NULL");
        return;
    }

    encoder_remove_obj_group(gui_water_heater.temperature.ui->arc_water_heater);
    ui_water_heater_delete(gui_water_heater.temperature.ui);
    gui_water_heater.temperature.ui = NULL;
}