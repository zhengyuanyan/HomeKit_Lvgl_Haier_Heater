
#include "gui_motor_cb.h"
#include "../gui_system/gui_system.h"
#include "esp_log.h"

static const char *TAG = "gui_system_motor_switch_cb";

void gui_system_motor_switch_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    /* ⚠️ 现在拿到的是组件，不是UI */
    gui_switch_t *sw = (gui_switch_t *)lv_event_get_user_data(e);

    if (!sw || !sw->ui)
    {
        ESP_LOGE(TAG, "sw or sw->ui is NULL");
        return;
    }

    /* =========================
     * 按键处理
     * ========================= */

    if (code == LV_EVENT_KEY)
    {
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

            /* ⭐ 状态同步（核心） */
            sw->value = ui_capsule_switch_get(sw->ui);

            ESP_LOGI(TAG, "motor: %d", sw->value);

            // 👉 推荐：这里发消息到控制层（比如队列）
            // gui_msg_post(MOTOR_SET, sw->value);

            break;

        case LV_KEY_ESC:
            gui_system_page_async();
            break;
        default:
            break;
        }
    }
}