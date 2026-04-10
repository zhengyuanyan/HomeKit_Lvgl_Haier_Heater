#include "ui_schedule_editor.h"
#include <stdlib.h>
#include <string.h>

#define roller_set_size_w 40
#define roller_set_size_h 80

/* ========================= Focus Style ========================= */
static lv_style_t style_focus;
static bool style_inited = false;

/* =========================
   时间选项
========================= */
static const char *hours =
    "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n"
    "13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";

static const char *mins =
    "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n"
    "14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n"
    "28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n"
    "42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n"
    "56\n57\n58\n59";

/* =========================
   SAVE
========================= */
static void save_cb(lv_event_t *e);
static void save_cb_from_encoder(ui_schedule_editor_t *ui);
static void ui_schedule_editor_update_focus(ui_schedule_editor_t *ui);
/* =========================
   CREATE
========================= */
ui_schedule_editor_t *ui_schedule_editor_create(lv_obj_t *parent, schedule_t *schedule, void (*on_saved)(void *user_data), void *user_data)
{
    ui_schedule_editor_t *ui = lv_malloc(sizeof(*ui));
    memset(ui, 0, sizeof(*ui));

    ui->schedule = schedule;
    ui->on_saved = on_saved;
    ui->user_data = user_data;

    ui->focus = FOCUS_START_H;
    ui->mode = MODE_FOCUS;

    if (!style_inited)
    {
        lv_style_init(&style_focus);
        lv_style_set_outline_width(&style_focus, 3);
        lv_style_set_outline_color(&style_focus, lv_color_hex(0x00FFAA));
        lv_style_set_outline_pad(&style_focus, 3);
        style_inited = true;
    }
    /* root */
    ui->root = lv_obj_create(parent);
    lv_obj_set_size(ui->root, 240, 240);

    lv_obj_clear_flag(ui->root, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->root, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_border_width(ui->root, 0, 0);
    lv_obj_set_style_pad_all(ui->root, 0, 0);
    lv_obj_set_style_radius(ui->root, 120, 0);
    lv_obj_set_style_bg_color(ui->root, lv_color_hex(0x1C1C1E), 0);

    /* rollers */
    ui->roller_start_h = lv_roller_create(ui->root);
    lv_roller_set_options(ui->roller_start_h, hours, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ui->roller_start_h, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ui->roller_start_h, 25, 70);

    ui->roller_start_m = lv_roller_create(ui->root);
    lv_roller_set_options(ui->roller_start_m, mins, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ui->roller_start_m, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ui->roller_start_m, 75, 70);

    ui->roller_end_h = lv_roller_create(ui->root);
    lv_roller_set_options(ui->roller_end_h, hours, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ui->roller_end_h, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ui->roller_end_h, 125, 70);

    ui->roller_end_m = lv_roller_create(ui->root);
    lv_roller_set_options(ui->roller_end_m, mins, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ui->roller_end_m, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ui->roller_end_m, 175, 70);

    lv_roller_set_selected(ui->roller_start_h, schedule->start_hour, LV_ANIM_OFF);
    lv_roller_set_selected(ui->roller_start_m, schedule->start_min, LV_ANIM_OFF);
    lv_roller_set_selected(ui->roller_end_h, schedule->end_hour, LV_ANIM_OFF);
    lv_roller_set_selected(ui->roller_end_m, schedule->end_min, LV_ANIM_OFF);

    /* switch */
    ui->sw_enable = lv_switch_create(ui->root);
    lv_obj_align(ui->sw_enable, LV_ALIGN_TOP_MID, 0, 10);

    if (schedule->enabled)
        lv_obj_add_state(ui->sw_enable, LV_STATE_CHECKED);

    /* save button */
    ui->btn_save = lv_btn_create(ui->root);
    lv_obj_set_size(ui->btn_save, 80, 35);
    lv_obj_align(ui->btn_save, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_t *label = lv_label_create(ui->btn_save);
    lv_label_set_text(label, "SAVE");
    lv_obj_center(label);

    lv_obj_add_event_cb(ui->btn_save, save_cb, LV_EVENT_CLICKED, ui);

    lv_obj_add_style(ui->roller_start_h, &style_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(ui->roller_start_m, &style_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(ui->roller_end_h, &style_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(ui->roller_end_m, &style_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(ui->sw_enable, &style_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(ui->btn_save, &style_focus, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_schedule_editor_update_focus(ui);
    return ui;
}

static void ui_schedule_editor_update_focus(ui_schedule_editor_t *ui)
{
    /* 先清掉所有 focused */
    lv_obj_clear_state(ui->roller_start_h, LV_STATE_FOCUSED);
    lv_obj_clear_state(ui->roller_start_m, LV_STATE_FOCUSED);
    lv_obj_clear_state(ui->roller_end_h, LV_STATE_FOCUSED);
    lv_obj_clear_state(ui->roller_end_m, LV_STATE_FOCUSED);
    lv_obj_clear_state(ui->sw_enable, LV_STATE_FOCUSED);
    lv_obj_clear_state(ui->btn_save, LV_STATE_FOCUSED);

    /* 再设置当前 focus */
    lv_obj_t *target = NULL;

    switch (ui->focus)
    {
    case FOCUS_START_H:
        target = ui->roller_start_h;
        break;
    case FOCUS_START_M:
        target = ui->roller_start_m;
        break;
    case FOCUS_END_H:
        target = ui->roller_end_h;
        break;
    case FOCUS_END_M:
        target = ui->roller_end_m;
        break;
    case FOCUS_SWITCH:
        target = ui->sw_enable;
        break;
    case FOCUS_SAVE:
        target = ui->btn_save;
        break;
    default:
        return;
    }

    if (target)
        lv_obj_add_state(target, LV_STATE_FOCUSED);
}

/* =========================
   DELETE
========================= */
void ui_schedule_editor_delete(ui_schedule_editor_t *ui)
{
    if (!ui)
        return;

    if (ui->root)
        lv_obj_del(ui->root);

    lv_free(ui);
}

/* =========================
   FOCUS SWITCH
========================= */
static void focus_next(ui_schedule_editor_t *ui)
{
    if (ui->focus < FOCUS_MAX - 1)
    {
        ui->focus++;
    }
    else
    {
        ui->focus = 0;
    }

    ui_schedule_editor_update_focus(ui);
}

static void focus_prev(ui_schedule_editor_t *ui)
{
    if (ui->focus > 0)
    {
        ui->focus--;
    }
    else
    {
        ui->focus = FOCUS_MAX - 1;
    }
    ui_schedule_editor_update_focus(ui);
}

/* =========================
   LEFT (FOCUS / EDIT)
========================= */
void ui_schedule_editor_left(ui_schedule_editor_t *ui)
{
    if (!ui)
        return;

    if (ui->mode == MODE_FOCUS)
    {
        focus_prev(ui);
        return;
    }

    /* EDIT MODE */
    lv_obj_t *obj = NULL;
    int max = 24;

    switch (ui->edit_target)
    {
    case FOCUS_START_H:
        obj = ui->roller_start_h;
        max = 24;
        break;
    case FOCUS_START_M:
        obj = ui->roller_start_m;
        max = 60;
        break;
    case FOCUS_END_H:
        obj = ui->roller_end_h;
        max = 24;
        break;
    case FOCUS_END_M:
        obj = ui->roller_end_m;
        max = 60;
        break;
    default:
        return;
    }

    int cur = lv_roller_get_selected(obj);
    cur--;
    if (cur < 0)
        cur = max - 1;

    lv_roller_set_selected(obj, cur, LV_ANIM_ON);
}

/* =========================
   RIGHT (FOCUS / EDIT)
========================= */
void ui_schedule_editor_right(ui_schedule_editor_t *ui)
{
    if (!ui)
        return;

    if (ui->mode == MODE_FOCUS)
    {
        focus_next(ui);
        return;
    }

    /* EDIT MODE */
    lv_obj_t *obj = NULL;
    int max = 24;

    switch (ui->edit_target)
    {
    case FOCUS_START_H:
        obj = ui->roller_start_h;
        max = 24;
        break;
    case FOCUS_START_M:
        obj = ui->roller_start_m;
        max = 60;
        break;
    case FOCUS_END_H:
        obj = ui->roller_end_h;
        max = 24;
        break;
    case FOCUS_END_M:
        obj = ui->roller_end_m;
        max = 60;
        break;
    default:
        return;
    }

    int cur = lv_roller_get_selected(obj);
    cur++;
    if (cur >= max)
        cur = 0;

    lv_roller_set_selected(obj, cur, LV_ANIM_ON);
}

/* =========================
   ENTER
========================= */
void ui_schedule_editor_enter(ui_schedule_editor_t *ui)
{
    if (!ui)
        return;

    if (ui->mode == MODE_FOCUS)
    {
        switch (ui->focus)
        {
        case FOCUS_START_H:
        case FOCUS_START_M:
        case FOCUS_END_H:
        case FOCUS_END_M:

            ui->mode = MODE_EDIT;
            ui->edit_target = ui->focus;
            ui_schedule_editor_update_focus(ui);
            return;

        case FOCUS_SWITCH:
            if (lv_obj_has_state(ui->sw_enable, LV_STATE_CHECKED))
                lv_obj_clear_state(ui->sw_enable, LV_STATE_CHECKED);
            else
                lv_obj_add_state(ui->sw_enable, LV_STATE_CHECKED);
            return;

        case FOCUS_SAVE:
            save_cb_from_encoder(ui);
            return;
        }
    }

    if (ui->mode == MODE_EDIT)
    {
        ui->mode = MODE_FOCUS;
    }
}

/* =========================
   SAVE
========================= */
static void save_cb_from_encoder(ui_schedule_editor_t *ui)
{
    char buf[8];

    lv_roller_get_selected_str(ui->roller_start_h, buf, sizeof(buf));
    ui->schedule->start_hour = atoi(buf);

    lv_roller_get_selected_str(ui->roller_start_m, buf, sizeof(buf));
    ui->schedule->start_min = atoi(buf);

    lv_roller_get_selected_str(ui->roller_end_h, buf, sizeof(buf));
    ui->schedule->end_hour = atoi(buf);

    lv_roller_get_selected_str(ui->roller_end_m, buf, sizeof(buf));
    ui->schedule->end_min = atoi(buf);

    ui->schedule->enabled =
        lv_obj_has_state(ui->sw_enable, LV_STATE_CHECKED);

    ui_schedule_editor_delete(ui);

    if (ui->on_saved)
        ui->on_saved(ui->user_data);
}

/* =========================
   SAVE BUTTON
========================= */
static void save_cb(lv_event_t *e)
{
    ui_schedule_editor_t *ui = lv_event_get_user_data(e);
    save_cb_from_encoder(ui);
}