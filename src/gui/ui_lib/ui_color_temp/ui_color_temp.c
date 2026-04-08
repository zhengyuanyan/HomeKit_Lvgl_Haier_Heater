#include "ui_color_temp.h"
#include "../images/lv_ui_images.h"
#include "../fonts/lv_ui_font.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 色温弧形滑块事件回调函数
 */
static void arc_color_temp_event(lv_event_t *event)
{
    ui_color_temp_t *ui = lv_event_get_user_data(event);

    uint32_t value = lv_arc_get_value(ui->arc_color_temp);

    ui->color_temp_value = value;

    char buf[16];
    // sprintf(buf, "%dK", value);
    lv_snprintf(buf, sizeof(buf), "%dK", value);
    lv_label_set_text(ui->label_color_temp, buf);
}

/*
 * 创建色温控件
 */
ui_color_temp_t *ui_color_temp_create(lv_obj_t *parent, uint32_t temp_k)
{

    ui_color_temp_t *ui = lv_malloc(sizeof(ui_color_temp_t));

    if (!ui)
    {
        LV_LOG_WARN("ui_color_temp_t malloc: ui is NULL");
        return NULL;
    }

    // memset(ui, 0, sizeof(ui_color_temp_t));

    if (temp_k < 2700)
    {
        temp_k = 2700;
    }
    else if (temp_k > 6500)
    {
        temp_k = 6500;
    }

    // 创建容器
    ui->cont = lv_obj_create(parent);
    lv_obj_set_size(ui->cont, 240, 240);
    lv_obj_set_style_bg_color(ui->cont, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->cont, 0, 0);
    lv_obj_set_style_pad_all(ui->cont, 0, 0);
    lv_obj_set_style_radius(ui->cont, 120, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 创建色温弧形滑块
    ui->arc_color_temp = lv_arc_create(ui->cont);
    lv_obj_set_pos(ui->arc_color_temp, 20, 20);
    lv_obj_set_size(ui->arc_color_temp, 200, 200);
    lv_arc_set_range(ui->arc_color_temp, 2700, 6500);
    lv_obj_center(ui->arc_color_temp);
    lv_obj_set_style_arc_width(ui->arc_color_temp, 15, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_image_src(ui->arc_color_temp, &Color_temperature_arc, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->arc_color_temp, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_arc_color(ui->arc_color_temp, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->arc_color_temp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->arc_color_temp, false, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->arc_color_temp, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_arc_set_value(ui->arc_color_temp, temp_k);
    lv_obj_add_event_cb(ui->arc_color_temp, arc_color_temp_event, LV_EVENT_VALUE_CHANGED, ui);

    // 创建色温值标签
    ui->label_color_temp = lv_label_create(ui->cont);
    // lv_obj_set_pos(ui->label_color_temp, 0, 0);
    lv_obj_center(ui->label_color_temp);
    lv_obj_set_style_text_color(ui->label_color_temp, lv_color_hex(0xEBEBF5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->label_color_temp, &lv_font_number_font_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->label_color_temp, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    char buf[16];
    // sprintf(buf, "%dK", temp_k);
    lv_snprintf(buf, sizeof(buf), "%dK", temp_k);
    lv_label_set_text(ui->label_color_temp, buf);

    ui->is_init = true;
    
    return ui;
}

/*
 * 删除色温控件
 */
void ui_color_temp_delete(ui_color_temp_t *ui)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_color_temp_delete: ui is NULL");
        return;
    }
    lv_obj_del(ui->cont);
    
    lv_free(ui);

}

/*
 * 设置色温值
 */
void ui_color_temp_set_value(ui_color_temp_t *ui, uint32_t temp_k)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_color_temp_set_value: ui is NULL");
        return;
    }
    // 更新弧形滑块值
    if (temp_k < 2700)
    {
        temp_k = 2700;
    }
    else if (temp_k > 6500)
    {
        temp_k = 6500;
    }

    ui->color_temp_value = temp_k;

    lv_arc_set_value(ui->arc_color_temp, temp_k);

    // 更新标签显示
    char buf[16];
    // sprintf(buf, "%dK", temp_k);
    lv_snprintf(buf, sizeof(buf), "%dK", temp_k);
    lv_label_set_text(ui->label_color_temp, buf);
}
/*
 * 获取色温值
 */
uint32_t ui_color_temp_get_value(ui_color_temp_t *ui)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_color_temp_get_value: ui is NULL");
        return 0;
    }

    return ui->color_temp_value;
}
