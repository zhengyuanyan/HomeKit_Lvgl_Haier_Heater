#include "ui_water_heater.h"
#include "../images/lv_ui_images.h"
#include "../fonts/lv_ui_font.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 水加热弧形滑块事件回调函数
 */
static void ui_water_heater_event_cb(lv_event_t *event)
{
    ui_water_heater_t *ui = lv_event_get_user_data(event);

    uint8_t value = lv_arc_get_value(ui->arc_water_heater); // 获取当前弧形滑块值
    ui->water_heater_value = value;

    char buf[16];
    // sprintf(buf, "%d°", value);
    lv_snprintf(buf, sizeof(buf), "%d°", value);
    lv_label_set_text(ui->label_water_heater, buf);
}

/*
 * 创建水加热控件
 */
ui_water_heater_t *ui_water_heater_create(lv_obj_t *parent, uint8_t water_heater_k)
{

    ui_water_heater_t *ui = lv_malloc(sizeof(ui_water_heater_t));

    if (!ui)
    {
        LV_LOG_WARN("ui_water_heater_t malloc: ui is NULL");
        return NULL;
    }

    memset(ui, 0, sizeof(ui_water_heater_t));

    if (water_heater_k < 35)
    {
        water_heater_k = 35;
    }
    else if (water_heater_k > 70)
    {
        water_heater_k = 70;
    }

    // 创建容器
    ui->cont = lv_obj_create(parent);
    lv_obj_set_size(ui->cont, 240, 240);
    lv_obj_set_style_bg_color(ui->cont, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->cont, 0, 0);
    lv_obj_set_style_pad_all(ui->cont, 0, 0);
    lv_obj_set_style_radius(ui->cont, 120, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 创建水加热弧形滑块
    ui->arc_water_heater = lv_arc_create(ui->cont);
    lv_obj_set_pos(ui->arc_water_heater, 20, 20);
    lv_obj_set_size(ui->arc_water_heater, 200, 200);
    lv_arc_set_range(ui->arc_water_heater, 35, 70);
    lv_obj_center(ui->arc_water_heater);
    lv_obj_set_style_arc_width(ui->arc_water_heater, 15, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_image_src(ui->arc_water_heater, &Water_heater_arc, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->arc_water_heater, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_arc_color(ui->arc_water_heater, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->arc_water_heater, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->arc_water_heater, false, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->arc_water_heater, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_arc_set_value(ui->arc_water_heater, water_heater_k);
    lv_obj_add_event_cb(ui->arc_water_heater, ui_water_heater_event_cb, LV_EVENT_VALUE_CHANGED, ui);

    // 创建水加热值标签
    ui->label_water_heater = lv_label_create(ui->cont);
    // lv_obj_set_pos(ui->label_water_heater, 0, 0);
    lv_obj_center(ui->label_water_heater);
    lv_obj_set_style_text_color(ui->label_water_heater, lv_color_hex(0xEBEBF5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->label_water_heater, &lv_font_number_font_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->label_water_heater, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    char buf[16];
    // sprintf(buf, "%d°", water_heater_k);
    lv_snprintf(buf, sizeof(buf), "%d°", water_heater_k);
    lv_label_set_text(ui->label_water_heater, buf);

    return ui;
}

/*
 * 删除水加热控件
 */
void ui_water_heater_delete(ui_water_heater_t *ui)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_water_heater_delete: ui is NULL");
        return;
    }
    lv_obj_del(ui->cont);

    lv_free(ui);
}

/*
 * 设置水加热值
 */
void ui_water_heater_set_value(ui_water_heater_t *ui, uint8_t water_heater_k)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_water_heater_set_value: ui is NULL");
        return;
    }
    // 更新弧形滑块值
    if (water_heater_k < 35)
    {
        water_heater_k = 35;
    }
    else if (water_heater_k > 65)
    {
        water_heater_k = 65;
    }

    ui->water_heater_value = water_heater_k;

    lv_arc_set_value(ui->arc_water_heater, water_heater_k);

    // 更新标签显示
    char buf[16];
    // sprintf(buf, "%dK", temp_k);
    lv_snprintf(buf, sizeof(buf), "%dK", water_heater_k);
    lv_label_set_text(ui->label_water_heater, buf);
}
/*
 * 获取水加热值
 */
uint8_t ui_water_heater_get_value(ui_water_heater_t *ui)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_water_heater_get_value: ui is NULL");
        return 0;
    }

    return ui->water_heater_value;
}
