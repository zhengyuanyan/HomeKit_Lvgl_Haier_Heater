#include "gui_buzzer.h"
#include "gui_buzzer_cb.h"
#include "esp_log.h"

static const char *TAG = "gui_system_buzzer";

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

/* =========================
 * 页面创建
 * ========================= */

void gui_system_buzzer_page(lv_obj_t *parent)
{
    if (!parent)
    {
        ESP_LOGE(TAG, "parent is NULL");
        return;
    }

    /* 创建开关组件 */
    gui.system.buzzer.ui = ui_capsule_switch_create(parent);

    if (!gui.system.buzzer.ui)
    {
        ESP_LOGE(TAG, "switch create failed");
        return;
    }

    /* 同步状态（关键点） */
    ui_capsule_switch_set(gui.system.buzzer.ui, gui.system.buzzer.value);

    /* encoder 绑定（⚠️ 传组件，不是UI） */
    encoder_add_focus_obj_group_event(gui.system.buzzer.ui->cont, gui_system_buzzer_switch_cb, &gui.system.buzzer);

    gui.system.buzzer.active = true;

    ESP_LOGI(TAG, "system buzzer page created run");
}

/* =========================
 * 页面删除
 * ========================= */

void gui_system_buzzer_page_delete(void)
{
    if (!gui.system.buzzer.ui)
        return;

    encoder_remove_obj_group(gui.system.buzzer.ui->cont);

    ui_capsule_switch_delete(gui.system.buzzer.ui);

    gui.system.buzzer.ui = NULL;

    gui.system.buzzer.active = false;
}
