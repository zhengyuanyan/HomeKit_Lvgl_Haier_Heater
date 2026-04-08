#include "ui_brightness.h"
#include "../images/lv_ui_images.h"
#include "../fonts/lv_ui_font.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * 亮度弧形滑块事件回调函数
 */
static void ui_brightness_event_cb(lv_event_t *event)
{
    ui_brightness_t *ui = lv_event_get_user_data(event);

    uint8_t value = lv_arc_get_value(ui->arc_brightness);

    ui->brightness_value = value;

    char buf[16];
    // sprintf(buf, "%d%%", value);
    lv_snprintf(buf, sizeof(buf), "%d%%", value);
    lv_label_set_text(ui->label_brightness, buf);
}

/*
 * 创建亮度控件
 */

ui_brightness_t *ui_brightness_create(lv_obj_t *parent, uint8_t brightness_k)
{

    ui_brightness_t *ui = lv_malloc(sizeof(ui_brightness_t));

    if (!ui)
    {
        LV_LOG_WARN("ui_brightness_t malloc: ui is NULL");
        return NULL;
    }

    // memset(ui, 0, sizeof(ui_brightness_t));

    if (brightness_k < 0)
    {
        brightness_k = 0;
    }
    else if (brightness_k > 100)
    {
        brightness_k = 100;
    }

    // 创建容器
    ui->cont = lv_obj_create(parent);
    
    lv_obj_set_size(ui->cont, 240, 240);
    lv_obj_set_style_bg_color(ui->cont, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->cont, 0, 0);
    lv_obj_set_style_pad_all(ui->cont, 0, 0);
    lv_obj_set_style_radius(ui->cont, 120, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 创建亮度弧形滑块
    ui->arc_brightness = lv_arc_create(ui->cont);
    lv_obj_set_pos(ui->arc_brightness, 20, 20);
    lv_obj_set_size(ui->arc_brightness, 200, 200);
    lv_arc_set_range(ui->arc_brightness, 0, 100);
    lv_obj_center(ui->arc_brightness);
    lv_obj_set_style_arc_width(ui->arc_brightness, 15, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_image_src(ui->arc_brightness, &Brightness_arc, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->arc_brightness, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_arc_color(ui->arc_brightness, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->arc_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->arc_brightness, false, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->arc_brightness, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_arc_set_value(ui->arc_brightness, brightness_k);
    lv_obj_add_event_cb(ui->arc_brightness, ui_brightness_event_cb, LV_EVENT_VALUE_CHANGED, ui);

    // 创建亮度值标签
    ui->label_brightness = lv_label_create(ui->cont);
    // lv_obj_set_pos(ui->label_brightness, 0, 0);
    lv_obj_center(ui->label_brightness);
    lv_obj_set_style_text_color(ui->label_brightness, lv_color_hex(0xEBEBF5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->label_brightness, &lv_font_number_font_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->label_brightness, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    char buf[16];
    // sprintf(buf, "%d%%", brightness_k);
    lv_snprintf(buf, sizeof(buf), "%d%%", brightness_k);
    lv_label_set_text(ui->label_brightness, buf);

    return ui;
}

/*
 * 删除亮度控件
 */
void ui_brightness_delete(ui_brightness_t *ui)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_brightness_delete: ui is NULL");
        return;
    }
    lv_obj_del(ui->cont);

    lv_free(ui);
}
/*
 * 设置亮度值
 */
void ui_brightness_set_value(ui_brightness_t *ui, uint8_t brightness_k)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_brightness_set_value: ui is NULL");
        return;
    }

    if (brightness_k < 0)
    {
        brightness_k = 0;
    }
    else if (brightness_k > 100)
    {
        brightness_k = 100;
    }

    lv_arc_set_value(ui->arc_brightness, brightness_k);

    ui->brightness_value = brightness_k;

    char buf[16];
    // sprintf(buf, "%d%%", brightness_k);
    lv_snprintf(buf, sizeof(buf), "%d%%", brightness_k);
    lv_label_set_text(ui->label_brightness, buf);
}

/*
 * 获取亮度值
 */
uint8_t ui_brightness_get_value(ui_brightness_t *ui)
{
    if (!ui)
    {
        LV_LOG_WARN("ui_brightness_get_value: ui is NULL");
        return 0;
    }
    return ui->brightness_value;
}