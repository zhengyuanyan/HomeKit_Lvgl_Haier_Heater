#include "gui_motor.h"
#include "gui_motor_cb.h"
#include "esp_log.h"

static const char *TAG = "gui_system_motor";

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

/* =========================
 * 页面创建
 * ========================= */

void gui_system_motor_page(lv_obj_t *parent)
{
    if (!parent)
    {
        ESP_LOGE(TAG, "parent is NULL");
        return;
    }

    /* 创建开关组件 */
    gui.system.motor.ui = ui_capsule_switch_create(parent);

    if (!gui.system.motor.ui)
    {
        ESP_LOGE(TAG, "switch create failed");
        return;
    }

    /* 同步状态（核心变化点） */
    ui_capsule_switch_set(gui.system.motor.ui, gui.system.motor.value);

    /* encoder 绑定 */
    encoder_add_focus_obj_group_event(gui.system.motor.ui->cont, gui_system_motor_switch_cb, &gui.system.motor); // ⚠️ 传组件，而不是 ui

    gui.system.motor.active = true;

    ESP_LOGI(TAG, "system motor page created run");
}

/* =========================
 * 页面删除
 * ========================= */

void gui_system_motor_page_delete(void)
{
    if (gui.system.motor.ui != NULL)
    {
        if (gui.system.motor.ui->cont)
        {
            encoder_remove_obj_group(gui.system.motor.ui->cont);
        }

        ui_capsule_switch_delete(gui.system.motor.ui);

        gui.system.motor.ui = NULL;
        gui.system.motor.active = false;
    }
    else
    {
        ESP_LOGW(TAG, "gui system motor UI is NULL");
        return;
    }
}