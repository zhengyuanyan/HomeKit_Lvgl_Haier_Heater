#include "gui_schedule_editor.h"
#include "gui_schedule_editor_cb.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "schedule_editor";

/* =========================
 * UI内部结构
 * ========================= */
struct gui_schedule_editor_s
{
    gui_knob_t knob;
    lv_obj_t *root;

    gui_schedule_ui_state_t state;
    uint8_t selected;

    bool active;
};

/* =========================
 * 全局实例
 * ========================= */
static gui_schedule_editor_t g_ui;
static schedule_mgr_t g_schedule_mgr;

/* encoder bind protection */
static bool encoder_bound = false;

/* =========================
 * DATA API
 * ========================= */
schedule_mgr_t *schedule_mgr_get(void)
{
    return &g_schedule_mgr;
}

schedule_t *schedule_get(uint8_t id)
{
    if (id >= SCHEDULE_MAX)
        return NULL;
    return &g_schedule_mgr.items[id];
}

void schedule_set_dirty(void)
{
    g_schedule_mgr.dirty = true;
}

/* =========================
 * default data
 * ========================= */
static void schedule_init_default(void)
{
    g_schedule_mgr.items[0] = (schedule_t){8, 0, 12, 0, true};
    g_schedule_mgr.items[1] = (schedule_t){13, 0, 18, 0, true};
    g_schedule_mgr.items[2] = (schedule_t){19, 0, 22, 0, true};
    g_schedule_mgr.items[3] = (schedule_t){0, 0, 0, 0, false};

    g_schedule_mgr.inited = true;
}

/* =========================
 * refresh API
 * ========================= */
void gui_schedule_editor_refresh(void)
{
    ui_sync();
}

/* =========================
 * editor save callback
 * ========================= */
static void on_editor_saved(void *user_data)
{
    (void)user_data;

    schedule_set_dirty();

    g_ui.state = UI_STATE_LIST;
    gui_schedule_editor_refresh();
}

/* =========================
 * open editor async
 * ========================= */
static void open_editor_async(void *arg)
{
    int id = (int)(intptr_t)arg;

    schedule_t *s = schedule_get(id);
    if (!s)
        return;

    g_ui.state = UI_STATE_EDIT;
    g_ui.selected = id;

    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);

    ui_schedule_editor_create(scr, s, on_editor_saved, NULL);
}

/* =========================
 * list click
 * ========================= */
static void open_editor_cb(lv_event_t *e)
{
    int id = (int)(intptr_t)lv_event_get_user_data(e);
    lv_async_call(open_editor_async, (void *)(intptr_t)id);
}




/* =========================
 * create UI
 * ========================= */
static void gui_schedule_editor_create(void)
{
    if (g_ui.active)
        return;

    if (!g_schedule_mgr.inited)
        schedule_init_default();

    g_ui.root = lv_scr_act();
    lv_obj_clean(g_ui.root);
    lv_obj_set_style_bg_color(g_ui.root, lv_color_hex(0x1C1C1E), 0);

    g_ui.state = UI_STATE_LIST;
    g_ui.selected = 0;

    g_ui.knob.ui = ui_segment_knob_create(g_ui.root);
    if (!g_ui.knob.ui)
    {
        ESP_LOGE(TAG, "knob create failed");
        return;
    }

    ui_sync();

    encoder_add_focus_obj_group_event(g_ui.knob.ui->cont, gui_schedule_editor_page_knob_event_cb, &g_ui);

    g_ui.active = true;
}



void gui_schedule_editor_page(void)
{
    lv_async_call((lv_async_cb_t)gui_schedule_editor_create, NULL);
}

static void gui_schedule_editor_page_delete_async(void *arg)
{
    (void)arg;

    if (g_ui.knob.ui && g_ui.knob.ui->cont)
    {
        encoder_remove_obj_group(g_ui.knob.ui->cont);
    }

    encoder_bound = false;

    if (g_ui.knob.ui)
    {
        ui_segment_knob_delete(g_ui.knob.ui);
        g_ui.knob.ui = NULL;
    }

    g_ui.active = false;
}

void gui_schedule_editor_page_delete(void)
{
    lv_async_call(gui_schedule_editor_page_delete_async, NULL);
}
