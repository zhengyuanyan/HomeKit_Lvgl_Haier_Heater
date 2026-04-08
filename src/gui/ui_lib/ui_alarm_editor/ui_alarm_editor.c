#include "ui_alarm_editor.h"
#include <stdio.h>
#include <stdlib.h>

/* =========================
 * 私有结构体
 * ========================= */
struct ui_alarm_editor
{
    lv_obj_t *cont;
    lv_obj_t *label;

    int hour;
    int min;
    alarm_edit_state_t state;

    alarm_done_cb_t cb;
    void *user;
};

/* =========================
 * UI刷新
 * ========================= */
static void refresh(ui_alarm_editor_t *ui)
{
    char buf[32];

    if (ui->state == ALARM_EDIT_HOUR)
        snprintf(buf, sizeof(buf), "[%02d]:%02d", ui->hour, ui->min);
    else if (ui->state == ALARM_EDIT_MIN)
        snprintf(buf, sizeof(buf), "%02d:[%02d]", ui->hour, ui->min);
    else
        snprintf(buf, sizeof(buf), "%02d:%02d OK?", ui->hour, ui->min);

    lv_label_set_text(ui->label, buf);
}

/* =========================
 * create
 * ========================= */
ui_alarm_editor_t *ui_alarm_editor_create(lv_obj_t *parent, int hour, int min, alarm_done_cb_t cb, void *user)
{
    ui_alarm_editor_t *ui = lv_malloc(sizeof(*ui));
    if (!ui)
        return NULL;

    lv_memzero(ui, sizeof(*ui));

    ui->hour = hour;
    ui->min = min;
    ui->state = ALARM_EDIT_HOUR;

    ui->cb = cb;
    ui->user = user;

    /* UI */
    ui->cont = lv_obj_create(parent);
    lv_obj_set_size(ui->cont, 220, 120);
    lv_obj_center(ui->cont);

    ui->label = lv_label_create(ui->cont);
    lv_obj_center(ui->label);

    refresh(ui);
    return ui;
}

/* =========================
 * encoder
 * ========================= */
void ui_alarm_editor_encoder(ui_alarm_editor_t *ui, int diff)
{
    if (!ui)
        return;

    if (ui->state == ALARM_EDIT_HOUR)
        ui->hour = (ui->hour + diff + 24) % 24;
    else if (ui->state == ALARM_EDIT_MIN)
        ui->min = (ui->min + diff + 60) % 60;

    refresh(ui);
}

/* =========================
 * click
 * ========================= */
void ui_alarm_editor_click(ui_alarm_editor_t *ui)
{
    if (!ui)
        return;

    if (ui->state == ALARM_EDIT_HOUR)
    {
        ui->state = ALARM_EDIT_MIN;
    }
    else if (ui->state == ALARM_EDIT_MIN)
    {
        ui->state = ALARM_EDIT_CONFIRM;
    }
    else
    {
        /* confirm */
        if (ui->cb)
            ui->cb(ui->hour, ui->min, ui->user);

        ui_alarm_editor_delete(ui);
        return;
    }

    refresh(ui);
}

/* =========================
 * delete
 * ========================= */
void ui_alarm_editor_delete(ui_alarm_editor_t *ui)
{
    if (!ui)
        return;

    if (ui->cont)
        lv_obj_del(ui->cont);

    lv_free(ui);
}