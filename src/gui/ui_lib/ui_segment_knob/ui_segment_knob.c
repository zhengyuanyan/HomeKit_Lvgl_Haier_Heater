#include "ui_segment_knob.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../icon/lv_ui_icon.h"
#include "../fonts/lv_ui_font.h"

#define SIZE 240
#define RADIUS 110

static void ui_segment_refresh(ui_segment_knob_t *ui);
static void ui_segment_highlight(ui_segment_knob_t *ui);

static const lv_color_t color_wheel[] = {
    LV_COLOR_MAKE(255, 99, 132),  /* 红 */
    LV_COLOR_MAKE(54, 162, 235),  /* 蓝 */
    LV_COLOR_MAKE(255, 206, 86),  /* 黄 */
    LV_COLOR_MAKE(75, 192, 192),  /* 青 */
    LV_COLOR_MAKE(153, 102, 255), /* 紫 */
    LV_COLOR_MAKE(255, 159, 64),  /* 橙 */
    LV_COLOR_MAKE(46, 204, 113),  /* 绿 */
    LV_COLOR_MAKE(231, 76, 60),   /* 朱红 */
    LV_COLOR_MAKE(52, 73, 94),    /* 深蓝 */
    LV_COLOR_MAKE(241, 196, 15),  /* 金 */
    LV_COLOR_MAKE(230, 126, 34),  /* 橘 */
    LV_COLOR_MAKE(26, 188, 156),  /* 绿松 */
};

ui_segment_knob_t *ui_segment_knob_create(lv_obj_t *parent)
{

    ui_segment_knob_t *ui = lv_malloc(sizeof(ui_segment_knob_t));

    if (!ui)
    {
        LV_LOG_WARN("ui_segment_knob_t malloc: ui is NULL");
        return NULL;
    }

    memset(ui, 0, sizeof(ui_segment_knob_t));

    ui->cont = lv_obj_create(parent);

    lv_obj_remove_style_all(ui->cont);

    lv_obj_set_size(ui->cont, SIZE, SIZE);
    lv_obj_center(ui->cont);

    return ui;
}

static void ui_segment_refresh(ui_segment_knob_t *ui)
{
    if (!ui || ui->item_count == 0)
        return;

    lv_obj_clean(ui->cont);

    ui->center_label = NULL;

    for (int i = 0; i < UI_SEGMENT_MAX_ITEMS; i++)
    {
        ui->arc[i] = NULL;
        ui->icon[i] = NULL;
    }

    int count = ui->item_count;

    float step = 360.0f / count;

    int CX = 120;
    int CY = 120;

    for (int i = 0; i < count; i++)
    {
        int start = i * step;
        // int end = start + step;
        int end = (i + 1) * step + 1;

        float mid_angle = (start + end) / 2.0f;

        // 转换角度为弧度
        float rad = mid_angle * M_PI / 180.0f;

        lv_obj_t *arc = lv_arc_create(ui->cont);

        lv_obj_remove_style_all(arc);

        lv_obj_set_size(arc, SIZE, SIZE);
        lv_obj_center(arc);

        lv_arc_set_range(arc, 0, 100);
        lv_arc_set_value(arc, 100);

        lv_arc_set_bg_angles(arc, start, end);

        /* 扇形厚度 */
        lv_obj_set_style_arc_width(arc, 80, LV_PART_MAIN);

        /* 扇形颜色 */
        // lv_obj_set_style_arc_color(arc, color_wheel[i], LV_PART_MAIN);
        lv_color_t c = color_wheel[i % (sizeof(color_wheel) / sizeof(color_wheel[0]))];
        ui->item_color[i] = c;
        lv_obj_set_style_arc_color(arc, c, LV_PART_MAIN);
        // lv_obj_set_style_arc_color(arc, c, LV_PART_INDICATOR);
        lv_obj_set_style_arc_rounded(arc, false, LV_PART_INDICATOR);
        lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_INDICATOR);

        /* 边界线 */
        lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_MAIN);

        ui->arc[i] = arc;

        // ICON
        if (ui->items[i].icon)
        {
            lv_obj_t *icon = lv_label_create(ui->cont);

            lv_label_set_text(icon, (const char *)ui->items[i].icon);

            lv_obj_set_style_text_font(icon, &ui_segment_knob_icon_24, 0);
            lv_obj_set_style_text_color(icon, lv_color_white(), 0);

            float icon_radius = RADIUS * 0.75f;

            // int ix = CX + cosf(mid_angle * M_PI / 180.0f) * icon_radius;
            // int iy = CY - sinf(mid_angle * M_PI / 180.0f) * icon_radius;

            int ix = CX + (int)(cosf(rad) * icon_radius);
            int iy = CY + (int)(sinf(rad) * icon_radius);

            lv_obj_align(icon, LV_ALIGN_CENTER, ix - CX, iy - CY);

            ui->icon[i] = icon;
        }
    }

    for (int i = 0; i < ui->item_count; i++)
    {
        if (ui->icon[i])
            lv_obj_move_foreground(ui->icon[i]);
    }

    if (!ui->center_label)
    {
        ui->center_label = lv_label_create(ui->cont);
        lv_obj_set_style_text_font(ui->center_label, &ui_segment_knob_font_20, 0);
        lv_obj_set_style_text_color(ui->center_label, lv_color_white(), 0);

        lv_obj_center(ui->center_label);

        if (ui->items[ui->current_index].name)
        {
            // lv_label_set_text(ui->center_label, ui->items[ui->current_index].name);
            const char *text = ui->items[ui->current_index].name ? ui->items[ui->current_index].name : "";
            lv_label_set_text(ui->center_label, text);
        }
    }

    ui_segment_highlight(ui);
}

static void ui_segment_highlight(ui_segment_knob_t *ui)
{
    if (!ui)
        return;

    for (int i = 0; i < ui->item_count; i++)
    {
        if (!ui->arc[i])
            continue;

        lv_color_t c = ui->item_color[i];

        if (i == ui->current_index)
        {
            lv_color_t highlight = lv_color_lighten(c, 40);

            lv_obj_set_style_arc_color(ui->arc[i], highlight, LV_PART_MAIN);

            lv_obj_set_style_arc_width(ui->arc[i], 56, LV_PART_MAIN);
        }
        else
        {
            lv_obj_set_style_arc_color(ui->arc[i], c, LV_PART_MAIN);

            lv_obj_set_style_arc_width(ui->arc[i], 60, LV_PART_MAIN);
        }
    }

    if (ui->center_label && ui->items[ui->current_index].name)
    {
        // lv_label_set_text(ui->center_label, ui->items[ui->current_index].name);
        const char *text = ui->items[ui->current_index].name ? ui->items[ui->current_index].name : "";
        lv_label_set_text(ui->center_label, text);

        lv_color_t c = lv_color_lighten(ui->item_color[ui->current_index], 40);
        lv_obj_set_style_text_color(ui->center_label, c, LV_PART_MAIN);
    }
}

void ui_segment_knob_add_item(ui_segment_knob_t *ui, const char *name, const void *icon, void (*enter_cb)(void *), void *user_data)
{
    if (!ui)
        return;

    if (ui->item_count >= UI_SEGMENT_MAX_ITEMS)
        return;

    int id = ui->item_count;

    ui->items[id].name = name;
    ui->items[id].icon = icon;
    ui->items[id].enter_cb = enter_cb;
    ui->items[id].user_data = user_data;

    ui->item_count++;

    ui_segment_refresh(ui);
}

void ui_segment_knob_next(ui_segment_knob_t *ui)
{
    if (!ui || ui->item_count == 0)
        return;

    ui->current_index++;

    if (ui->current_index >= ui->item_count)
        ui->current_index = 0;

    ui_segment_highlight(ui);
}

void ui_segment_knob_prev(ui_segment_knob_t *ui)
{
    if (!ui || ui->item_count == 0)
        return;

    ui->current_index--;

    if (ui->current_index < 0)
        ui->current_index = ui->item_count - 1;

    ui_segment_highlight(ui);
}

void ui_segment_knob_enter(ui_segment_knob_t *ui)
{
    if (!ui || ui->item_count == 0)
        return;

    ui_segment_item_t *item = &ui->items[ui->current_index];

    if (item->enter_cb)
        item->enter_cb(item->user_data);
}

void ui_segment_knob_delete(ui_segment_knob_t *ui)
{
    if (!ui)
        return;

    /* 删除 LVGL 对象树 */
    if (ui->cont)
    {
        lv_obj_delete(ui->cont);
        ui->cont = NULL;
    }

    /* 清空状态 */
    ui->item_count = 0;
    ui->current_index = 0;

    /* 释放结构体 */

    lv_free(ui);
}

void ui_segment_knob_clear(ui_segment_knob_t *ui)
{
    if (!ui)
        return;

    lv_obj_clean(ui->cont);

    ui->item_count = 0;
    ui->current_index = 0;

    ui->center_label = NULL;

    for (int i = 0; i < UI_SEGMENT_MAX_ITEMS; i++)
    {
        ui->arc[i] = NULL;
        ui->icon[i] = NULL;
    }
}
