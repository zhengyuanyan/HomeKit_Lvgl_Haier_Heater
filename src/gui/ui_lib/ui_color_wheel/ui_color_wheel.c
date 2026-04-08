#include "ui_color_wheel.h"
#include "lvgl.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>

/***********************
 * 内部函数声明
 ***********************/
static void cont_event_cb(lv_event_t *e);
static void handle_wheel_press(ui_color_wheel_t *ui, int32_t rel_x, int32_t rel_y);
static void handle_brightness_press(ui_color_wheel_t *ui, int32_t rel_x, int32_t rel_y, bool pressed);
static void get_pixel(const lv_img_dsc_t *img, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b);
static void wheel_set_hue(ui_color_wheel_t *ui, float hue);

/***********************
 * 创建控件
 ***********************/
ui_color_wheel_t *ui_color_wheel_create(lv_obj_t *parent, const lv_img_dsc_t *wheel_img)
{
    ui_color_wheel_t *ui = lv_malloc(sizeof(ui_color_wheel_t));
    LV_ASSERT_MALLOC(ui);
    memset(ui, 0, sizeof(ui_color_wheel_t));

    ui->brightness = 100;
    ui->brightness_dragging = false;
    ui->start_brightness = 0;
    ui->last_angle = 0;
    ui->mode = WHEEL_MODE_COLOR;
    ui->hue = 0;
    // 容器
    ui->cont = lv_obj_create(parent);
    lv_obj_set_size(ui->cont, 240, 240);
    lv_obj_clear_flag(ui->cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui->cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(ui->cont, lv_color_hex(0x1C1C1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->cont, 0, 0);
    lv_obj_set_style_pad_all(ui->cont, 0, 0);
    lv_obj_set_style_radius(ui->cont, 120, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 色轮图片
    ui->wheel = lv_img_create(ui->cont);
    lv_img_set_src(ui->wheel, wheel_img);
    lv_obj_center(ui->wheel);
    lv_obj_clear_flag(ui->wheel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_set_style_radius(ui->wheel, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wheel, LV_OPA_0, 0);

    // 指示器
    ui->indicator = lv_obj_create(ui->cont);
    lv_obj_set_size(ui->indicator, 12, 12);
    lv_obj_set_style_bg_color(ui->indicator, lv_color_white(), 0);
    lv_obj_set_style_border_color(ui->indicator, lv_color_black(), 0);
    lv_obj_set_style_border_width(ui->indicator, 2, 0);
    lv_obj_set_style_radius(ui->indicator, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(ui->indicator, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui->indicator, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_pos(ui->indicator, 120 - 6, 120 - 6); // 初始中心

    // 亮度环
    ui->brightness_arc = lv_arc_create(ui->cont);
    lv_obj_set_size(ui->brightness_arc, 220, 220);
    lv_arc_set_range(ui->brightness_arc, 0, 100);
    lv_arc_set_value(ui->brightness_arc, 100);
    lv_obj_center(ui->brightness_arc);
    lv_obj_clear_flag(ui->brightness_arc, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE);

    // 事件
    lv_obj_add_event_cb(ui->cont, cont_event_cb, LV_EVENT_ALL, ui);

    return ui;
}

/***********************
 * 删除控件
 ***********************/
void ui_color_wheel_delete(ui_color_wheel_t *ui)
{
    if (!ui)
        return;
    if (ui->cont)
        lv_obj_del(ui->cont);
    lv_free(ui);
}

/***********************
 * 设置颜色
 ***********************/
static float rgb_to_hue(uint8_t r, uint8_t g, uint8_t b)
{
    float fr = r / 255.0f;
    float fg = g / 255.0f;
    float fb = b / 255.0f;
    float max = fmaxf(fmaxf(fr, fg), fb);
    float min = fminf(fminf(fr, fg), fb);
    float delta = max - min;
    if (delta < 0.0001f)
        return 0.0f;

    float hue = 0;
    if (max == fr)
        hue = 60 * fmodf((fg - fb) / delta, 6);
    else if (max == fg)
        hue = 60 * ((fb - fr) / delta + 2);
    else
        hue = 60 * ((fr - fg) / delta + 4);
    if (hue < 0)
        hue += 360;
    return hue;
}

void ui_color_wheel_set_color(ui_color_wheel_t *ui, uint8_t r, uint8_t g, uint8_t b)
{
    if (!ui || !ui->wheel)
        return;

    const lv_img_dsc_t *img = (const lv_img_dsc_t *)lv_img_get_src(ui->wheel);
    if (!img || !img->data)
        return;

    float hue = rgb_to_hue(r, g, b);

    int cx = img->header.w / 2;
    int cy = img->header.h / 2;
    float radius = (float)cx - 1;

    float rad = hue * M_PI / 180.0f;
    int px = (int)(cx - radius * sinf(rad));
    int py = (int)(cy + radius * cosf(rad));

    if (px < 0)
        px = 0;
    if (px >= (int)img->header.w)
        px = img->header.w - 1;
    if (py < 0)
        py = 0;
    if (py >= (int)img->header.h)
        py = img->header.h - 1;

    get_pixel(img, px, py, &ui->r, &ui->g, &ui->b);
    lv_obj_set_pos(ui->indicator, px + (120 - cx) - 6, py + (120 - cy) - 6);
}

/***********************
 * 获取颜色
 ***********************/
void ui_color_wheel_get_color(ui_color_wheel_t *ui, uint8_t *r, uint8_t *g, uint8_t *b)
{
    if (!ui)
        return;
    *r = ui->r * ui->brightness / 100;
    *g = ui->g * ui->brightness / 100;
    *b = ui->b * ui->brightness / 100;
}

/***********************
 * 设置 / 获取亮度
 ***********************/
void ui_color_wheel_set_brightness(ui_color_wheel_t *ui, uint8_t brightness)
{
    if (!ui)
        return;
    if (brightness > 100)
        brightness = 100;
    ui->brightness = brightness;
    lv_arc_set_value(ui->brightness_arc, brightness);
}

uint8_t ui_color_wheel_get_brightness(ui_color_wheel_t *ui)
{
    if (!ui)
        return 0;
    return ui->brightness;
}

/***********************
 * 内部函数：读取像素
 ***********************/
static void get_pixel(const lv_img_dsc_t *img, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint16_t *buf = (uint16_t *)img->data;
    uint16_t c = buf[y * img->header.w + x];
    *r = ((c >> 11) & 0x1F) << 3;
    *g = ((c >> 5) & 0x3F) << 2;
    *b = (c & 0x1F) << 3;
}

/***********************
 * 内部函数：事件回调
 ***********************/
static void cont_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_PRESSED && code != LV_EVENT_PRESSING && code != LV_EVENT_RELEASED)
        return;

    ui_color_wheel_t *ui = lv_event_get_user_data(e);
    lv_indev_t *indev = lv_event_get_indev(e);
    if (!indev)
        return;

    lv_point_t p;
    lv_indev_get_point(indev, &p);

    lv_area_t area;
    lv_obj_get_coords(ui->cont, &area);

    int rel_x = p.x - area.x1;
    int rel_y = p.y - area.y1;

    int cx = 120, cy = 120;
    float dx = rel_x - cx;
    float dy = rel_y - cy;
    float dist = sqrtf(dx * dx + dy * dy);

    if (code == LV_EVENT_PRESSED || code == LV_EVENT_PRESSING)
    {
        if (dist <= 70)
            handle_wheel_press(ui, rel_x, rel_y);
        else if (dist > 70 && dist <= 120)
            handle_brightness_press(ui, rel_x, rel_y, code == LV_EVENT_PRESSED);
    }
    else if (code == LV_EVENT_RELEASED)
    {
        ui->brightness_dragging = false;
    }
}

/***********************
 * 内部函数：处理色轮
 ***********************/
static void handle_wheel_press(ui_color_wheel_t *ui, int32_t rel_x, int32_t rel_y)
{
    const lv_img_dsc_t *img = (const lv_img_dsc_t *)lv_img_get_src(ui->wheel);
    if (!img || !img->data)
        return;

    int img_cx = img->header.w / 2;
    int img_cy = img->header.h / 2;

    int px = rel_x - (120 - img_cx);
    int py = rel_y - (120 - img_cy);

    if (px < 0)
        px = 0;
    if (px >= (int)img->header.w)
        px = img->header.w - 1;
    if (py < 0)
        py = 0;
    if (py >= (int)img->header.h)
        py = img->header.h - 1;

    get_pixel(img, px, py, &ui->r, &ui->g, &ui->b);
    lv_obj_set_pos(ui->indicator, px + (120 - img_cx) - 6, py + (120 - img_cy) - 6);
}

/***********************
 * 内部函数：处理亮度环
 ***********************/
static void handle_brightness_press(ui_color_wheel_t *ui, int32_t rel_x, int32_t rel_y, bool pressed)
{
    int cx = 120, cy = 120;
    float dx = rel_x - cx;
    float dy = rel_y - cy;
    float angle = atan2f(dy, dx);

    if (pressed)
    {
        ui->last_angle = angle;
        ui->start_brightness = ui->brightness;
        ui->brightness_dragging = true;
    }
    else if (ui->brightness_dragging)
    {
        float delta = angle - ui->last_angle;
        if (delta > M_PI)
            delta -= 2 * M_PI;
        if (delta < -M_PI)
            delta += 2 * M_PI;

        int new_brightness = ui->start_brightness + (int)(delta / (2 * M_PI) * 100.0f + 0.5f);
        if (new_brightness < 0)
            new_brightness = 0;
        if (new_brightness > 100)
            new_brightness = 100;

        ui->brightness = (uint8_t)new_brightness;
        lv_arc_set_value(ui->brightness_arc, new_brightness);
    }
}

static void wheel_set_hue(ui_color_wheel_t *ui, float hue)
{
    const lv_img_dsc_t *img = (const lv_img_dsc_t *)lv_img_get_src(ui->wheel);
    if(!img) return;

    ui->hue = hue;

    int cx = img->header.w / 2;
    int cy = img->header.h / 2;
    float radius = cx - 1;

    float rad = hue * M_PI / 180.0f;

    int px = cx - radius * sinf(rad);
    int py = cy + radius * cosf(rad);

    uint8_t r,g,b;
    get_pixel(img, px, py, &r,&g,&b);

    ui->r = r;
    ui->g = g;
    ui->b = b;

    lv_obj_set_pos(ui->indicator, px + (120 - cx) - 6, py + (120 - cy) - 6);
}

void ui_color_wheel_encoder(ui_color_wheel_t *ui, int diff)
{
    if(ui->mode == WHEEL_MODE_COLOR)
    {
        ui->hue += diff * 3;

        if(ui->hue < 0) ui->hue += 360;
        if(ui->hue >= 360) ui->hue -= 360;

        wheel_set_hue(ui, ui->hue);
    }
    else
    {
        int b = ui->brightness + diff;

        if(b < 0) b = 0;
        if(b > 100) b = 100;

        ui->brightness = b;

        lv_arc_set_value(ui->brightness_arc, b);
    }
}

void ui_color_wheel_encoder_enter(ui_color_wheel_t *ui)
{
    if(ui->mode == WHEEL_MODE_COLOR)
        ui->mode = WHEEL_MODE_BRIGHTNESS;
    else
        ui->mode = WHEEL_MODE_COLOR;
}