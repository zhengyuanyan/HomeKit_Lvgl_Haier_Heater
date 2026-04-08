#include "gui_heater_switch.h"
#include "gui_heater_switch_cb.h"
#include "esp_log.h"

static const char *TAG = "gui_heater_switch";

/* =========================
 * 全局对象（组件化）
 * ========================= */

gui_heater_t gui_heater_switch = {
    .heater = {
        .ui = NULL,
        .value = false,
    },
};

/* =========================
 * 页面创建
 * ========================= */

void gui_heater_switch_page(lv_obj_t *parent)
{
    if (!parent)
    {
        ESP_LOGE(TAG, "parent is NULL");
        return;
    }

    /* 创建开关组件 */
    gui_heater_switch.heater.ui = ui_capsule_switch_create(parent);

    if (!gui_heater_switch.heater.ui)
    {
        ESP_LOGE(TAG, "switch create failed");
        return;
    }

    /* 同步状态（关键点） */
    ui_capsule_switch_set(gui_heater_switch.heater.ui, gui_heater_switch.heater.value);

    /* encoder 绑定（⚠️ 传组件） */
    encoder_add_focus_obj_group_event(gui_heater_switch.heater.ui->cont, gui_heater_switch_cb, &gui_heater_switch.heater);
}

/* =========================
 * 页面删除
 * ========================= */

void gui_heater_switch_page_delete(void)
{
    if (!gui_heater_switch.heater.ui)
    {
        ESP_LOGE(TAG, "gui_heater_switch.heater.ui is NULL");
        return;
    }

    encoder_remove_obj_group(gui_heater_switch.heater.ui->cont);

    ui_capsule_switch_delete(gui_heater_switch.heater.ui);

    gui_heater_switch.heater.ui = NULL;
}