// #include "gui_buzzer.h"
// #include "gui_buzzer_cb.h"
// #include "esp_log.h"

// static const char *TAG = "gui_system_buzzer";

// gui_system_buzzer_t gui_system_buzzer = {
//     .ui_capsule_switch = NULL,
// };

// gui_system_buzzer_switch_data_t gui_system_buzzer_switch_data = {
//     .gui_system_buzzer_switch_onoff = false};

// /**
//  * @brief 创建蜂鸣器系统界面
//  * @param parent 父级对象指针，用于创建界面元素
//  * @note 此函数用于创建蜂鸣器系统的控制界面，包括开关等元素
//  */
// void gui_system_buzzer_page(lv_obj_t *parent)
// {
//     // 检查父级对象是否有效
//     if (!parent)
//     {
//         ESP_LOGE(TAG, " gui_system_buzzer_page parent is NULL");
//         return;
//     }
//     // 创建胶囊形状的开关控件
//     gui_system_buzzer.ui_capsule_switch = ui_capsule_switch_create(parent);

//     // 检查开关控件是否创建成功
//     if (!gui_system_buzzer.ui_capsule_switch)
//     {
//         ESP_LOGE(TAG, "gui_system_buzzer.ui_capsule_switch failed");
//         return;
//     }
//     // 初始化开关状态，使用全局变量中的加热器开关状态
//     ui_capsule_switch_set(gui_system_buzzer.ui_capsule_switch, gui_system_buzzer_switch_data.gui_system_buzzer_switch_onoff);

//     // 添加 encoder 事件
//     encoder_add_focus_obj_group_event(gui_system_buzzer.ui_capsule_switch->cont, gui_system_buzzer_switch_cb, gui_system_buzzer.ui_capsule_switch);
// }

// void gui_system_buzzer_page_delete(void)
// {
//     if (!gui_system_buzzer.ui_capsule_switch)
//     {
//         ESP_LOGE(TAG, "!gui_system_buzzer.ui_capsule_switch is NULL");
//         return;
//     }
//     encoder_remove_obj_group(gui_system_buzzer.ui_capsule_switch->cont);
//     ui_capsule_switch_delete(gui_system_buzzer.ui_capsule_switch);
//     gui_system_buzzer.ui_capsule_switch = NULL;
// }

#include "gui_buzzer.h"
#include "gui_buzzer_cb.h"
#include "esp_log.h"

static const char *TAG = "gui_system_buzzer";

/* =========================
 * 全局对象（组件化）
 * ========================= */

gui_system_buzzer_t gui_system_buzzer = {
    .buzzer = {
        .ui = NULL,
        .value = false,
    },
};

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
    gui_system_buzzer.buzzer.ui = ui_capsule_switch_create(parent);

    if (!gui_system_buzzer.buzzer.ui)
    {
        ESP_LOGE(TAG, "switch create failed");
        return;
    }

    /* 同步状态（关键点） */
    ui_capsule_switch_set(
        gui_system_buzzer.buzzer.ui,
        gui_system_buzzer.buzzer.value);

    /* encoder 绑定（⚠️ 传组件，不是UI） */
    encoder_add_focus_obj_group_event(
        gui_system_buzzer.buzzer.ui->cont,
        gui_system_buzzer_switch_cb,
        &gui_system_buzzer.buzzer);
}

/* =========================
 * 页面删除
 * ========================= */

void gui_system_buzzer_page_delete(void)
{
    if (!gui_system_buzzer.buzzer.ui)
        return;

    encoder_remove_obj_group(gui_system_buzzer.buzzer.ui->cont);

    ui_capsule_switch_delete(gui_system_buzzer.buzzer.ui);

    gui_system_buzzer.buzzer.ui = NULL;
}
