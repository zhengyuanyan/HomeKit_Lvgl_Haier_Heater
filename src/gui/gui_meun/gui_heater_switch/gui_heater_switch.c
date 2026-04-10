#include "gui_heater_switch.h"
#include "gui_heater_switch_cb.h"
#include "esp_log.h"

static const char *TAG = "gui_heater_switch";

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

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

    /* 防止重复创建 */
    if (gui.heater.heater.ui)
    {
        ESP_LOGW(TAG, "heater UI already created");
        return;
    }

    /* 创建开关组件 */
    gui.heater.heater.ui = ui_capsule_switch_create(parent);

    if (!gui.heater.heater.ui)
    {
        ESP_LOGE(TAG, "switch create failed");
        return;
    }

    /* 同步状态 */
    ui_capsule_switch_set(gui.heater.heater.ui, gui.heater.heater.value);

    /* encoder绑定 */
    encoder_add_focus_obj_group_event(gui.heater.heater.ui->cont, gui_heater_switch_cb, &gui.heater.heater);

    gui.heater.active = true;

    ESP_LOGI(TAG, "heater page created");
}

/* =========================
 * 页面删除
 * ========================= */

void gui_heater_switch_page_delete(void)
{

    if (gui.heater.heater.ui != NULL)
    {
        if (gui.heater.heater.ui->cont)
        {
            encoder_remove_obj_group(gui.heater.heater.ui->cont);
        }

        ui_capsule_switch_delete(gui.heater.heater.ui);
        gui.heater.heater.ui = NULL;
        gui.heater.active = false;
    }
    else
    {
        ESP_LOGW(TAG, "heater UI is NULL");
        return;
    }
}