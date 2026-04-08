#include "ui_capsule_switch.h"
#include <string.h>
#include <stdio.h>

#define SWITCH_W 180
#define SWITCH_H 60

#define SLIDER_W 80
#define SLIDER_H 52

#define ANIM_TIME 180

static void switch_refresh(ui_capsule_switch_t *ui);
static void slider_anim(lv_obj_t *obj, int32_t start, int32_t end);

/*********************
 * 创建组件
 *********************/
ui_capsule_switch_t *ui_capsule_switch_create(lv_obj_t *parent)
{
    ui_capsule_switch_t *ui = lv_malloc(sizeof(ui_capsule_switch_t));
    if (!ui)
    {
        LV_LOG_WARN("ui_capsule_switch_t malloc failed\n");
        return NULL;
    }
        

    memset(ui, 0, sizeof(ui_capsule_switch_t));

    ui->state = false;
    ui->focus = 0;

    /* container */
    ui->cont = lv_obj_create(parent);
    lv_obj_remove_style_all(ui->cont);

    lv_obj_set_size(ui->cont, SWITCH_W, SWITCH_H);

    lv_obj_set_style_radius(ui->cont, SWITCH_H / 2, 0);
    lv_obj_set_style_bg_color(ui->cont, lv_color_hex(0x2A2A2A), 0);

    lv_obj_set_style_border_width(ui->cont, 2, 0);
    lv_obj_set_style_border_color(ui->cont, lv_color_hex(0x555555), 0);

    lv_obj_center(ui->cont);

    /* OFF */
    ui->label_off = lv_label_create(ui->cont);
    lv_label_set_text(ui->label_off, "OFF");
    lv_obj_align(ui->label_off, LV_ALIGN_LEFT_MID, 20, 0);

    /* ON */
    ui->label_on = lv_label_create(ui->cont);
    lv_label_set_text(ui->label_on, "ON");
    lv_obj_align(ui->label_on, LV_ALIGN_RIGHT_MID, -20, 0);

    /* slider */
    ui->slider = lv_obj_create(ui->cont);
    lv_obj_remove_style_all(ui->slider);

    lv_obj_set_size(ui->slider, SLIDER_W, SLIDER_H);
    lv_obj_set_style_radius(ui->slider, SLIDER_H / 2, 0);
    lv_obj_set_style_bg_color(ui->slider, lv_color_white(), 0);

    lv_obj_align(ui->slider, LV_ALIGN_LEFT_MID, 4, 0);

    lv_obj_clear_flag(ui->slider, LV_OBJ_FLAG_CLICKABLE);

    switch_refresh(ui);

    return ui;
}

/*********************
 * slider动画
 *********************/
static void slider_anim(lv_obj_t *obj, int32_t start, int32_t end)
{
    lv_anim_t a;
    lv_anim_init(&a);

    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, start, end);

    lv_anim_set_time(&a, ANIM_TIME);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);

    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

    lv_anim_start(&a);
}

/*********************
 * UI刷新
 *********************/
static void switch_refresh(ui_capsule_switch_t *ui)
{
    if (!ui)
        return;

    /* 高亮 */
    if (ui->focus == 0)
    {
        lv_obj_set_style_text_color(ui->label_off, lv_color_white(), 0);
        lv_obj_set_style_text_color(ui->label_on, lv_color_hex(0x888888), 0);

        /* focus glow */
        lv_obj_set_style_shadow_width(ui->label_off, 10, 0);
        lv_obj_set_style_shadow_color(ui->label_off, lv_color_white(), 0);

        lv_obj_set_style_shadow_width(ui->label_on, 0, 0);
    }
    else
    {
        lv_obj_set_style_text_color(ui->label_on, lv_color_hex(0xFFB84D), 0);
        lv_obj_set_style_text_color(ui->label_off, lv_color_hex(0x888888), 0);

        lv_obj_set_style_shadow_width(ui->label_on, 10, 0);
        lv_obj_set_style_shadow_color(ui->label_on, lv_color_hex(0xFFB84D), 0);

        lv_obj_set_style_shadow_width(ui->label_off, 0, 0);
    }

    /* slider位置 */
    int left = 4;
    int right = SWITCH_W - SLIDER_W - 4;

    int cur = lv_obj_get_x(ui->slider);

    if (ui->state)
        slider_anim(ui->slider, cur, right);
    else
        slider_anim(ui->slider, cur, left);

    /* ON glow */
    if (ui->state)
    {
        lv_obj_set_style_shadow_width(ui->cont, 40, 0);
        lv_obj_set_style_shadow_color(ui->cont, lv_color_hex(0xFFB84D), 0);
    }
    else
    {
        lv_obj_set_style_shadow_width(ui->cont, 0, 0);
    }
}

/*********************
 * 编码器左
 *********************/
void ui_capsule_switch_left(ui_capsule_switch_t *ui)
{
    if (!ui)
        return;

    ui->focus = 0;

    // printf("left focus = %d\n", ui->focus);

    switch_refresh(ui);
}

/*********************
 * 编码器右
 *********************/
void ui_capsule_switch_right(ui_capsule_switch_t *ui)
{
    if (!ui)
        return;

    ui->focus = 1;

    // printf("right focus = %d\n", ui->focus);

    switch_refresh(ui);
}

/*********************
 * 编码器按下
 *********************/
void ui_capsule_switch_enter(ui_capsule_switch_t *ui)
{
    if (!ui)
        return;

    if (ui->focus == 0)
        ui->state = false;
    else
        ui->state = true;

    switch_refresh(ui);
}

/*********************
 * 状态设置
 *********************/
void ui_capsule_switch_set(ui_capsule_switch_t *ui, bool on)
{
    if (!ui)
        return;

    ui->state = on;

    switch_refresh(ui);
}

/*********************
 * 状态获取
 *********************/
bool ui_capsule_switch_get(ui_capsule_switch_t *ui)
{
    if (!ui)
        return false;

    return ui->state;
}

/*********************
 * 删除
 *********************/
void ui_capsule_switch_delete(ui_capsule_switch_t *ui)
{
    if (!ui)
        return;

    if (ui->cont)
        lv_obj_del(ui->cont);

    lv_free(ui);
}