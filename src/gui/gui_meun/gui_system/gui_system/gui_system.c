// #include "gui_system.h"
// #include "gui_system_cb.h"
// #include "esp_log.h"
// #include "../gui_buzzer/gui_buzzer.h"
// #include "../gui_motor/gui_motor.h"

// static const char *TAG = "gui_system";

// gui_system_t gui_system = {
//     .ui_segment_knob = NULL,
// };

// typedef struct
// {
//     const char *name;
//     const void *icon;
// } gui_system_name_symbols_t;

// static const gui_system_name_symbols_t gui_system_name_symbols[] = {
//     {"系统信息", LV_SETUP_SYMBOL_CHIP},
//     {"WIFI", LV_SETUP_SYMBOL_WIFI},
//     {"HomeKit", LV_SETUP_SYMBOL_HOMEKIT},
//     {"开关时间", LV_SETUP_SYMBOL_TIMING},
//     {"声音", LV_SETUP_SYMBOL_BUZZER},
//     {"震动", LV_SETUP_SYMBOL_MOTOR},
//     {"屏幕背光", LV_SETUP_SYMBOL_BRIGHTNESS},
//     {"RGB灯", LV_SETUP_SYMBOL_WS2812},
// };

// #define SETUP_COUNT (sizeof(gui_system_name_symbols) / sizeof(gui_system_name_symbols[0]))

// static void open_page(void *arg);
// static void open_page_async(void *arg);
// static void gui_system_page_async_run(void *arg);
// static void gui_system_page_delete_async(void *arg);
// static void gui_system_heater_page_delete_async(void *arg);
// static void gui_system_heater_page_delete(void);

// void gui_system_page_async(void)
// {
//     lv_async_call(gui_system_page_async_run, NULL);
//     // gui_menu_page_async_run(NULL);
// }

// static void gui_system_page_async_run(void *arg)
// {
//     (void)arg;

//     if (gui_system.ui_segment_knob)
//     {
//         gui_system_page_delete();
//     }
//     gui_system_heater_page_delete();

//     lv_obj_t *scr = lv_scr_act();
//     lv_obj_clean(scr);
//     lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

//     gui_system_page(scr);
// }
// void gui_system_page(lv_obj_t *parent)
// {
//     gui_system.ui_segment_knob = ui_segment_knob_create(parent);

//     if (!gui_system.ui_segment_knob)
//     {
//         ESP_LOGE(TAG, "gui_system.ui_segment_knob failed");
//         return;
//     }

//     for (uint8_t i = 0; i < SETUP_COUNT; i++)
//     {
//         ui_segment_knob_add_item(gui_system.ui_segment_knob, gui_system_name_symbols[i].name, gui_system_name_symbols[i].icon, open_page, (void *)(intptr_t)i);
//     }

//     encoder_add_focus_obj_group_event(gui_system.ui_segment_knob->cont, gui_system_page_knob_event_cb, gui_system.ui_segment_knob);
// }

// static void open_page(void *arg)
// {
//     lv_async_call(open_page_async, arg);
//     // open_page_async(arg);
// }

// static void open_page_async(void *arg)
// {
//     int p = (int)(intptr_t)arg;

//     encoder_group_set_editing(false);

//     // 删除旧 UI
//     if (gui_system.ui_segment_knob)
//     {
//         gui_system_page_delete();
//     }
//     gui_system_heater_page_delete();

//     lv_obj_t *scr = lv_scr_act();
//     lv_obj_clean(scr);
//     lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

//     switch (p)
//     {
//     case 0:
//         ESP_LOGI(TAG, "系统设置");
//         break;
//     case 1:
//         ESP_LOGI(TAG, "WIFI");
//         break;
//     case 2:
//         ESP_LOGI(TAG, "HomeKit");
//         break;
//     case 3:
//         ESP_LOGI(TAG, "开关时间");
//         break;
//     case 4:
//         ESP_LOGI(TAG, "声音");
//         gui_system_buzzer_page(scr);
//         break;
//     case 5:
//         ESP_LOGI(TAG, "震动");
//         gui_system_motor_page(scr);
//         break;
//     case 6:
//         ESP_LOGI(TAG, "屏幕背光");
//         break;
//     case 7:
//         ESP_LOGI(TAG, "RGB灯");
//         break;
//     }
// }

// static void gui_system_page_delete_async(void *arg)
// {
//     (void)arg;
//     if (!gui_system.ui_segment_knob)
//     {
//         ESP_LOGE(TAG, "gui_system.ui_segment_knob is NULL");
//         return;
//     }

//     encoder_remove_obj_group(gui_system.ui_segment_knob->cont);
//     ui_segment_knob_delete(gui_system.ui_segment_knob);
//     gui_system.ui_segment_knob = NULL;
// }

// static void gui_system_heater_page_delete_async(void *arg)
// {
//     (void)arg;
//     if (gui_system_buzzer.ui_capsule_switch)
//     {
//         gui_system_buzzer_page_delete();
//     }
//     if (gui_system_motor.ui_capsule_switch)
//     {
//         gui_system_motor_page_delete();
//     }
// }

// static void gui_system_heater_page_delete(void)
// {
//     // lv_async_call(gui_menu_heater_delete_async, NULL);
//     gui_system_heater_page_delete_async(NULL);
// }
// void gui_system_page_delete(void)
// {
//     // lv_async_call(gui_system_page_delete_async, NULL);
//     gui_system_page_delete_async(NULL);
// }

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

    if (!gui.system.main.ui)
        return;

    encoder_remove_obj_group(gui.system.main.ui->cont);
    ui_segment_knob_delete(gui.system.main.ui);
    gui.system.main.ui = NULL;
    gui.system.main.active = false;
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