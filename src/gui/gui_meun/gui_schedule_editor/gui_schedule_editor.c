#include "gui_schedule_editor.h"
#include "gui_schedule_editor_cb.h"
#include "../../encoder_group_cb/encoder_group_cb.h"
#include "time.h"
#include "esp_log.h"

static const char *TAG = "gui_schedule_editor";

typedef struct
{
    const void *icon;
} gui_schedule_editor_main_item_t;

static const gui_schedule_editor_main_item_t main_items[] = {
    {LV_TIMING_SYMBOL_ONE},
    {LV_TIMING_SYMBOL_TWO},
    {LV_TIMING_SYMBOL_THREE},
    {LV_TIMING_SYMBOL_FOUR},
};

typedef struct
{
    char text[32];
} schedule_view_t;

static schedule_view_t view_cache[GUI_SCHEDULE_MAX];

/* =========================
 * 使用统一GUI入口
 * ========================= */
extern gui_t gui;

/* =========================
   模拟设备
========================= */
static void heater_on(void)
{
    printf("🔥 ON\n");
}

static void heater_off(void)
{
    printf("❄️ OFF\n");
}

/* =========================
   schedule执行逻辑（自己写）
========================= */
static void schedule_run(void)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int cur = t->tm_hour * 60 + t->tm_min;

    for (int i = 0; i < GUI_SCHEDULE_MAX; i++)
    {
        schedule_t *s = &gui.schedule.items[i];

        if (!s->enabled)
            continue;

        int start = s->start_hour * 60 + s->start_min;
        int end = s->end_hour * 60 + s->end_min;

        int active = (start <= end)
                         ? (cur >= start && cur < end)
                         : (cur >= start || cur < end);

        if (active && !s->state)
        {
            s->state = 1;
            if (s->on_cb)
                s->on_cb();
        }
        else if (!active && s->state)
        {
            s->state = 0;
            if (s->off_cb)
                s->off_cb();
        }
    }
}

static void build_schedule_cache(void)
{
    for (int i = 0; i < GUI_SCHEDULE_MAX; i++)
    {
        schedule_t *s = &gui.schedule.items[i];

        snprintf(view_cache[i].text, sizeof(view_cache[i].text),
                 "开: %02d:%02d\n关: %02d:%02d",
                 s->start_hour, s->start_min,
                 s->end_hour, s->end_min);
    }
}

/* =========================
   打开编辑器
========================= */

static void on_editor_saved(void *user_data)
{
    LV_UNUSED(user_data);

    /* 回到主页 */
    gui_schedule_editor_page(lv_scr_act());
}

static void open_editor_cb(void *data)
{
    int id = (int)(intptr_t)data;

    gui.schedule.active_index = id;
    /* 清屏 */
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);

    gui.schedule.ui = ui_schedule_editor_create(scr, &gui.schedule.items[id], on_editor_saved, NULL);

    encoder_add_focus_obj_group_event(gui.schedule.ui->root, gui_schedule_open_editor_page_event_cb, &gui.schedule);
}

/* =========================
   首页
========================= */

void gui_schedule_editor_page(lv_obj_t *parent)
{

    gui.schedule.main.ui = ui_segment_knob_create(parent);

    build_schedule_cache();


    if (!gui.schedule.main.ui)
    {
        ESP_LOGE(TAG, "knob create failed");
        return;
    }
    for (uint8_t i = 0; i < GUI_SCHEDULE_MAX; i++)
    {
        ui_segment_knob_add_item(gui.schedule.main.ui, view_cache[i].text, main_items[i].icon, open_editor_cb, (void *)(intptr_t)i);
    }

    encoder_add_focus_obj_group_event(gui.schedule.main.ui->cont, gui_schedule_editor_page_knob_event_cb, &gui.schedule.main);

    gui.schedule.active = true;

    ESP_LOGI(TAG, "schedule editor page run ");
}

/* =========================
 * 页面删除
 * ========================= */
void gui_schedule_editor_page_delete(void)
{

    if (gui.schedule.main.ui != NULL)
    {
        if (gui.schedule.main.ui->cont)
        {
            encoder_remove_obj_group(gui.schedule.main.ui->cont);
        }

        ui_segment_knob_delete(gui.schedule.main.ui);
        gui.schedule.main.ui = NULL;
    }

    gui.schedule.active = false;

    ESP_LOGI(TAG, "schedule page deleted");
}