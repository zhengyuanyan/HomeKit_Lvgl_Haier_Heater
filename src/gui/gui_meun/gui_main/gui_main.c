#include "gui_main.h"
#include "gui_main_cb.h"
#include "esp_log.h"
#include "../../encoder_group_cb/encoder_group_cb.h"
#include "../gui_heater_switch/gui_heater_switch.h"
#include "../gui_water_heater/gui_water_heater.h"
#include "../gui_system/gui_system/gui_system.h"
#include "../gui_schedule_editor/gui_schedule_editor.h"
#include "../gui_digital_tube_clock/gui_digital_tube_clock.h"

static const char *TAG = "gui_menu";

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

/* =========================
 * 菜单项定义（支持扩展）
 * ========================= */

typedef void (*page_func_t)(lv_obj_t *);

typedef struct
{
    const char *name;
    const void *icon;
    page_func_t page;
} gui_main_item_t;

static const gui_main_item_t main_items[] = {
    {"系统", LV_MENU_SYMBOL_SETTINGS, gui_system_page},
    {"加热", LV_MENU_SYMBOL_HEATING, gui_heater_switch_page},
    {"预约", LV_MENU_SYMBOL_TIMING, gui_schedule_editor_page},
    {"水温", LV_MENU_SYMBOL_TEMPERATURE, gui_water_heater_page},
};

#define MENU_COUNT (sizeof(main_items) / sizeof(main_items[0]))

/* =========================
 * 前向声明
 * ========================= */

static void open_page(void *arg);
static void open_page_async(void *arg);
static void gui_main_page_async_run(void *arg);

/* =========================
 * 页面入口
 * ========================= */

void gui_main_page(void)
{
    lv_async_call(gui_main_page_async_run, NULL);
}

/* =========================
 * 页面创建
 * ========================= */

static void gui_main_page_async_run(void *arg)
{
    (void)arg;

    gui_main_page_subpage_delete_all();

    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

    gui.main.knob.ui = ui_segment_knob_create(scr);

    if (!gui.main.knob.ui)
    {
        ESP_LOGE(TAG, "knob create failed");
        return;
    }

    for (uint8_t i = 0; i < MENU_COUNT; i++)
    {
        ui_segment_knob_add_item(gui.main.knob.ui, main_items[i].name, main_items[i].icon, open_page, (void *)(intptr_t)i);
    }

    encoder_add_focus_obj_group_event(gui.main.knob.ui->cont, gui_main_page_knob_event_cb, &gui.main);

    gui.main.active = true;

    ESP_LOGI(TAG, "gui_menu_page_async_run");
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
    if (gui.main.knob.ui)
    {
        gui_main_page_delete();
    }

    gui_main_page_subpage_delete_all();

    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1C1C1E), 0);

    /* ⭐ 无 switch 架构 */
    if (main_items[index].page)
    {
        main_items[index].page(scr);
    }
    else
    {
        ESP_LOGI(TAG, "page not implemented: %d", index);
    }
}

/* =========================
 * 删除逻辑（统一管理）
 * ========================= */

static void gui_main_page_delete_async(void *arg)
{
    (void)arg;

    if (gui.main.knob.ui != NULL)
    {
        if (gui.main.knob.ui->cont)
        {
            encoder_remove_obj_group(gui.main.knob.ui->cont);
        }

        ui_segment_knob_delete(gui.main.knob.ui);

        gui.main.knob.ui = NULL;
        gui.main.active = false;
    }
    else
    {
        ESP_LOGW(TAG, "main UI is NULL");
        return;
    }
}

void gui_main_page_delete(void)
{
    gui_main_page_delete_async(NULL);
}

/* =========================
 * 子页面统一释放（关键）
 * ========================= */

void gui_main_page_subpage_delete_all(void)
{
    if (gui.heater.heater.ui)
    {
        gui_heater_switch_page_delete();
    }

    if (gui.water_heater.temperature.ui)
    {
        gui_water_heater_page_delete();
    }

    if (gui.schedule.main.ui || gui.schedule.ui)
    {
        gui_schedule_editor_page_delete();
    }
    if (gui.clock.digital_tube.ui)
    {
        gui_digital_tube_clock_page_delete();
    }
}