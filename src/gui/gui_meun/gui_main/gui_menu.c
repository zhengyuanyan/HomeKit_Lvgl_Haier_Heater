#include "gui_menu.h"
#include "gui_menu_cb.h"
#include "esp_log.h"
#include "../../encoder_group_cb/encoder_group_cb.h"
#include "../gui_heater_switch/gui_heater_switch.h"
#include "../gui_water_heater/gui_water_heater.h"
#include "../gui_system/gui_system/gui_system.h"
#include "../gui_schedule_editor/gui_schedule_editor.h"

static const char *TAG = "gui_menu";

/* =========================
 * 全局对象（组件化）
 * ========================= */

gui_menu_t gui_menu = {
    .knob = {
        .ui = NULL,
        .value = 0,
    },
};

/* =========================
 * 菜单项定义（支持扩展）
 * ========================= */

typedef void (*page_func_t)(lv_obj_t *);

typedef struct
{
    const char *name;
    const void *icon;
    page_func_t page;
} gui_menu_item_t;

static const gui_menu_item_t menu_items[] = {
    {"系统", LV_MENU_SYMBOL_SETTINGS, gui_system_page},
    {"加热", LV_MENU_SYMBOL_HEATING, gui_heater_switch_page},
    {"预约", LV_MENU_SYMBOL_TIMING, gui_schedule_editor_page},
    {"水温", LV_MENU_SYMBOL_TEMPERATURE, gui_water_heater_page},
};

#define MENU_COUNT (sizeof(menu_items) / sizeof(menu_items[0]))

/* =========================
 * 前向声明
 * ========================= */

static void open_page(void *arg);
static void open_page_async(void *arg);
static void gui_menu_page_async_run(void *arg);

/* =========================
 * 页面入口
 * ========================= */

void gui_menu_page(void)
{
    lv_async_call(gui_menu_page_async_run, NULL);
}

/* =========================
 * 页面创建
 * ========================= */

static void gui_menu_page_async_run(void *arg)
{
    (void)arg;

    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

    gui_menu.knob.ui = ui_segment_knob_create(scr);

    if (!gui_menu.knob.ui)
    {
        ESP_LOGE(TAG, "knob create failed");
        return;
    }

    for (uint8_t i = 0; i < MENU_COUNT; i++)
    {
        ui_segment_knob_add_item(gui_menu.knob.ui, menu_items[i].name, menu_items[i].icon, open_page, (void *)(intptr_t)i);
    }

    encoder_add_focus_obj_group_event(gui_menu.knob.ui->cont, gui_menu_page_knob_event_cb, &gui_menu);
}

/* =========================
 * 页面跳转（核心优化）
 * ========================= */

static void open_page(void *arg)
{
    lv_async_call(open_page_async, arg);
}

static void open_page_async(void *data)
{
    int index = (int)(intptr_t)data;

    encoder_group_set_editing(false);

    /* 删除当前 UI */
    if (gui_menu.knob.ui)
    {
        gui_menu_page_delete();
    }

    gui_menu_subpage_delete_all();

    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

    /* ⭐ 无 switch 架构 */
    if (menu_items[index].page)
    {
        menu_items[index].page(scr);
    }
    else
    {
        ESP_LOGI(TAG, "page not implemented: %d", index);
    }
}

/* =========================
 * 删除逻辑（统一管理）
 * ========================= */

static void gui_menu_page_delete_async(void *arg)
{
    (void)arg;

    if (!gui_menu.knob.ui)
    {
        ESP_LOGE(TAG, "gui_menu.knob.ui is NULL");
        return;
    }

    encoder_remove_obj_group(gui_menu.knob.ui->cont);
    ui_segment_knob_delete(gui_menu.knob.ui);

    gui_menu.knob.ui = NULL;
}

void gui_menu_page_delete(void)
{
    gui_menu_page_delete_async(NULL);
}

/* =========================
 * 子页面统一释放（关键）
 * ========================= */

void gui_menu_subpage_delete_all(void)
{
    if (gui_heater_switch.heater.ui)
    {
        gui_heater_switch_page_delete();
    }

    if (gui_water_heater.temperature.ui)
    {
        gui_water_heater_page_delete();
    }
}