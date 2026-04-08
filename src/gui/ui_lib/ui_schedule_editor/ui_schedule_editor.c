#include "ui_schedule_editor.h"
#include <stdlib.h>
#include <string.h>

#define roller_set_size_w 40
#define roller_set_size_h 80
/* =========================
   时间选项
========================= */
static const char *hours =
    "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";

static const char *mins =
    "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";

/* =========================
   保存
========================= */
static void save_cb(lv_event_t *e)
{
    ui_schedule_editor_t *ed = lv_event_get_user_data(e);
    schedule_t *s = ed->schedule;

    char buf[8];

    lv_roller_get_selected_str(ed->roller_start_h, buf, sizeof(buf));
    s->start_hour = atoi(buf);

    lv_roller_get_selected_str(ed->roller_start_m, buf, sizeof(buf));
    s->start_min = atoi(buf);

    lv_roller_get_selected_str(ed->roller_end_h, buf, sizeof(buf));
    s->end_hour = atoi(buf);

    lv_roller_get_selected_str(ed->roller_end_m, buf, sizeof(buf));
    s->end_min = atoi(buf);

    s->enabled = lv_obj_has_state(ed->sw_enable, LV_STATE_CHECKED);
    s->state = 0;

    void (*cb)(void *) = ed->on_saved;
    void *ud = ed->user_data;

    /* 销毁当前编辑页面 */
    ui_schedule_editor_destroy(ed);
    if (cb)
        cb(ud);
}
/* ===== 调用回调跳转主界面 ===== */

/* =========================
   创建（240圆屏优化版）
========================= */
ui_schedule_editor_t *ui_schedule_editor_create(lv_obj_t *parent, schedule_t *schedule, void (*on_saved)(void *user_data), void *user_data)
{
    ui_schedule_editor_t *ed = lv_malloc(sizeof(*ed));
    memset(ed, 0, sizeof(*ed));

    ed->schedule = schedule;
    ed->on_saved = on_saved;
    ed->user_data = user_data;

    /* ===== root（圆屏适配） ===== */
    ed->root = lv_obj_create(parent);
    lv_obj_set_size(ed->root, 240, 240);
    lv_obj_center(ed->root);

    lv_obj_clear_flag(ed->root, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ed->root, LV_OBJ_FLAG_SCROLL_CHAIN);
    /* 👉 圆形视觉 */
    lv_obj_set_style_radius(ed->root, 120, 0);
    lv_obj_set_style_bg_color(ed->root, lv_color_hex(0x1C1C1E), 0);

    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_14);
    lv_style_set_bg_color(&style_sel, lv_color_hex(0x121212));
    lv_style_set_border_width(&style_sel, 2);
    lv_style_set_border_color(&style_sel, lv_color_hex3(0xf00));
    /* =========================
       START TIME（左上弧）
    ========================= */
    ed->roller_start_h = lv_roller_create(ed->root);
    lv_roller_set_options(ed->roller_start_h, hours, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ed->roller_start_h, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ed->roller_start_h, 10, 55);
    lv_roller_set_selected(ed->roller_start_h, schedule->start_hour, LV_ANIM_OFF);
    lv_obj_add_style(ed->roller_start_h, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_bg_color(ed->roller_start_h, lv_color_hex(0x1C1C1E), 0);
 

    ed->roller_start_m = lv_roller_create(ed->root);
    lv_roller_set_options(ed->roller_start_m, mins, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ed->roller_start_m, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ed->roller_start_m, 60, 55);
    lv_roller_set_selected(ed->roller_start_m, schedule->start_min, LV_ANIM_OFF);
    lv_obj_add_style(ed->roller_start_m, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_bg_color(ed->roller_start_m, lv_color_hex(0x1C1C1E), 0);

    /* =========================
       END TIME（右上弧）
    ========================= */
    ed->roller_end_h = lv_roller_create(ed->root);
    lv_roller_set_options(ed->roller_end_h, hours, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ed->roller_end_h, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ed->roller_end_h, 110, 55);
    lv_roller_set_selected(ed->roller_end_h, schedule->end_hour, LV_ANIM_OFF);
    lv_obj_add_style(ed->roller_end_h, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_bg_color(ed->roller_end_h, lv_color_hex(0x1C1C1E), 0);


    ed->roller_end_m = lv_roller_create(ed->root);
    lv_roller_set_options(ed->roller_end_m, mins, LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(ed->roller_end_m, roller_set_size_w, roller_set_size_h);
    lv_obj_set_pos(ed->roller_end_m, 160, 55);
    lv_roller_set_selected(ed->roller_end_m, schedule->end_min, LV_ANIM_OFF);
    lv_obj_add_style(ed->roller_end_m, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_bg_color(ed->roller_end_m, lv_color_hex(0x1C1C1E), 0);
 

    /* =========================
       CENTER SWITCH（核心状态）
    ========================= */
    ed->sw_enable = lv_switch_create(ed->root);
    lv_obj_center(ed->sw_enable);
    lv_obj_set_size(ed->sw_enable, 50, 25);
    lv_obj_align(ed->sw_enable, LV_ALIGN_TOP_MID, 0, -5);
    if (schedule->enabled)
        lv_obj_add_state(ed->sw_enable, LV_STATE_CHECKED);


    /* =========================
       SAVE BUTTON（底部居中）
    ========================= */
    ed->btn_save = lv_btn_create(ed->root);
    lv_obj_set_size(ed->btn_save, 80, 35);
    lv_obj_align(ed->btn_save, LV_ALIGN_BOTTOM_MID, 0, -2);


    lv_obj_t *label = lv_label_create(ed->btn_save);
    lv_label_set_text(label, "SAVE");
    lv_obj_center(label);

    lv_obj_add_event_cb(ed->btn_save, save_cb, LV_EVENT_CLICKED, ed);

    return ed;
}

/* =========================
   销毁
========================= */
void ui_schedule_editor_destroy(ui_schedule_editor_t *ed)
{
    if (!ed)
        return;

    if (ed->root)
        lv_obj_del(ed->root);

    lv_free(ed);
}