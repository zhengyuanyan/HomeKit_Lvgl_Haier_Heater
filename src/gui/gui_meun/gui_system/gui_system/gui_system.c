#include "gui_system.h"
#include "gui_system_cb.h"
#include "esp_log.h"
#include "../gui_buzzer/gui_buzzer.h"
#include "../gui_motor/gui_motor.h"

static const char *TAG = "gui_system";

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

/* =========================
 * 系统菜单项定义
 * ========================= */
typedef void (*page_func_t)(lv_obj_t *);
typedef struct
{
    const char *name;
    const void *icon;
    page_func_t page;
} gui_system_item_t;

static const gui_system_item_t items[] = {
    {"系统信息", LV_SETUP_SYMBOL_CHIP, NULL},
    {"WIFI", LV_SETUP_SYMBOL_WIFI, NULL},
    {"HomeKit", LV_SETUP_SYMBOL_HOMEKIT, NULL},
    {"开关时间", LV_SETUP_SYMBOL_TIMING, NULL},
    {"声音", LV_SETUP_SYMBOL_BUZZER, gui_system_buzzer_page},
    {"震动", LV_SETUP_SYMBOL_MOTOR, gui_system_motor_page},
    {"屏幕背光", LV_SETUP_SYMBOL_BRIGHTNESS, NULL},
    {"RGB灯", LV_SETUP_SYMBOL_WS2812, NULL},
};

#define ITEM_COUNT (sizeof(items) / sizeof(items[0]))

/* =========================
 * 前向声明
 * ========================= */
static void open_page(void *arg);
static void open_page_async(void *arg);
static void gui_system_page_async_run(void *arg);

/* =========================
 * 页面入口
 * ========================= */
void gui_system_page_async(void)
{
    lv_async_call(gui_system_page_async_run, NULL);
}

static void gui_system_page_async_run(void *arg)
{
    (void)arg;

    /* 删除当前 UI */
    if (gui.system.main.ui)
    {
        gui_system_page_delete();
    }

    gui_system_subpage_delete_all();

    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

    gui_system_page(scr);
}

/* =========================
 * 页面创建
 * ========================= */
void gui_system_page(lv_obj_t *parent)
{
    gui.system.main.ui = ui_segment_knob_create(parent);
    if (!gui.system.main.ui)
    {
        ESP_LOGE(TAG, "knob create failed");
        return;
    }

    for (uint8_t i = 0; i < ITEM_COUNT; i++)
    {
        ui_segment_knob_add_item(gui.system.main.ui, items[i].name, items[i].icon, open_page, (void *)(intptr_t)i);
    }

    encoder_add_focus_obj_group_event(gui.system.main.ui->cont, gui_system_page_knob_event_cb, gui.system.main.ui);

    gui.system.main.active = true;

    ESP_LOGI(TAG, "gui_system_page run");
}

/* =========================
 * 页面跳转（异步安全）
 * ========================= */
static void open_page(void *arg)
{
    lv_async_call(open_page_async, arg);
}

static void open_page_async(void *arg)
{
    int index = (int)(intptr_t)arg;

    encoder_group_set_editing(false);

    /* 删除当前 UI */
    if (gui.system.main.ui)
    {
        gui_system_page_delete();
    }

    gui_system_subpage_delete_all();

    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

    if (items[index].page)
    {
        items[index].page(scr);
    }
    else
    {
        ESP_LOGI(TAG, "page not implemented: %d", index);
    }
}

/* =========================
 * 页面删除
 * ========================= */
static void gui_system_page_delete_async(void *arg)
{
    (void)arg;

    if (gui.system.main.ui != NULL)
    {
        if (gui.system.main.ui->cont)
        {
            encoder_remove_obj_group(gui.system.main.ui->cont);
        }

        ui_segment_knob_delete(gui.system.main.ui);
        gui.system.main.ui = NULL;
        gui.system.main.active = false;
    }
    else
    {
        ESP_LOGE(TAG, "gui system main UI is NULL");
        return;
    }
}

void gui_system_page_delete(void)
{
    gui_system_page_delete_async(NULL);
}

/* =========================
 * 子页面统一删除
 * ========================= */
void gui_system_subpage_delete_all(void)
{
    if (gui.system.buzzer.ui)
    {
        gui_system_buzzer_page_delete();
    }

    if (gui.system.motor.ui)
    {
        gui_system_motor_page_delete();
    }

    /* 后续扩展直接加新的子页面删除 */
}