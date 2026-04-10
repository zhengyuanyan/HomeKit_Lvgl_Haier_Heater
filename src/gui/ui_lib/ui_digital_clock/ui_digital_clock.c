#include "ui_digital_clock.h"
#include <time.h>

/* ===== 图片资源 ===== */
extern const lv_image_dsc_t digital_tube_a;
extern const lv_image_dsc_t digital_tube_b;
extern const lv_image_dsc_t digital_tube_c;
extern const lv_image_dsc_t digital_tube_d;
extern const lv_image_dsc_t digital_tube_e;
extern const lv_image_dsc_t digital_tube_f;
extern const lv_image_dsc_t digital_tube_g;

extern const lv_image_dsc_t digital_tube_h_a;
extern const lv_image_dsc_t digital_tube_h_b;
extern const lv_image_dsc_t digital_tube_h_c;
extern const lv_image_dsc_t digital_tube_h_d;
extern const lv_image_dsc_t digital_tube_h_e;
extern const lv_image_dsc_t digital_tube_h_f;
extern const lv_image_dsc_t digital_tube_h_g;

/* ===== 段资源 ===== */
static const lv_image_dsc_t *seg_on[7] = {
    &digital_tube_a, &digital_tube_b, &digital_tube_c,
    &digital_tube_d, &digital_tube_e, &digital_tube_f, &digital_tube_g};

static const lv_image_dsc_t *seg_off[7] = {
    &digital_tube_h_a, &digital_tube_h_b, &digital_tube_h_c,
    &digital_tube_h_d, &digital_tube_h_e, &digital_tube_h_f, &digital_tube_h_g};

/* ===== 数码管编码 ===== */
static const uint8_t digit_lut[10] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
    0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};

/* ===== 获取段状态 ===== */
static inline uint8_t seg_onoff(uint8_t digit, int seg)
{
    if (digit > 9)
        return 0;
    return (digit_lut[digit] >> seg) & 0x01;
}

/* ===== 设置数字 ===== */
static void set_digit(ui_digital_clock_t *ui, int idx, int num)
{
    for (int i = 0; i < 7; i++)
    {
        lv_image_set_src(ui->seg[idx][i], seg_onoff(num, i) ? seg_on[i] : seg_off[i]);
    }
}

/* ===== digit 布局 ===== */
static void layout_digit(ui_digital_clock_t *ui, int idx)
{
    const int pos[7][2] = {
        {67, 97}, {85, 99}, {85, 121}, {67, 139}, {65, 121}, {65, 99}, {67, 118}};

    int min_x = pos[0][0];
    int min_y = pos[0][1];

    for (int i = 1; i < 7; i++)
    {
        if (pos[i][0] < min_x)
            min_x = pos[i][0];
        if (pos[i][1] < min_y)
            min_y = pos[i][1];
    }

    int max_x = 0, max_y = 0;

    for (int i = 0; i < 7; i++)
    {
        int x = pos[i][0] - min_x;
        int y = pos[i][1] - min_y;

        lv_obj_set_pos(ui->seg[idx][i], x, y);

        int w = LV_MAX(seg_on[i]->header.w, seg_off[i]->header.w);
        int h = LV_MAX(seg_on[i]->header.h, seg_off[i]->header.h);

        if (x + w > max_x)
            max_x = x + w;
        if (y + h > max_y)
            max_y = y + h;
    }

    lv_obj_set_size(ui->digit[idx], max_x, max_y);
}

/* ===== 定时器 ===== */
static void timer_cb(lv_timer_t *t)
{
    ui_digital_clock_t *ui = lv_timer_get_user_data(t);

    time_t now = time(NULL);
    struct tm tm_info;

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_info, &now);
#else
    localtime_r(&now, &tm_info);
#endif

    int hour = tm_info.tm_hour;
    int min = tm_info.tm_min;

    /* ===== 显示 ===== */
    set_digit(ui, 0, hour / 10);
    set_digit(ui, 1, hour % 10);
    set_digit(ui, 2, min / 10);
    set_digit(ui, 3, min % 10);

    /* ===== 冒号闪烁 ===== */
    ui->colon_on = !ui->colon_on;
    lv_opa_t opa = ui->colon_on ? LV_OPA_COVER : LV_OPA_30;

    for (int i = 0; i < COLON_COUNT; i++)
    {
        lv_obj_set_style_opa(ui->colon[i], opa, 0);
    }
}
/* ===== 创建 ===== */
ui_digital_clock_t *ui_digital_clock_create(lv_obj_t *parent)
{
    ui_digital_clock_t *ui = lv_malloc(sizeof(ui_digital_clock_t));
    if (!ui)
        return NULL;

    lv_memzero(ui, sizeof(ui_digital_clock_t));

    /* ===== 主容器 ===== */
    ui->cont = lv_obj_create(parent);
    lv_obj_center(ui->cont);
    lv_obj_set_size(ui->cont, 240, 240);
    lv_obj_clear_flag(ui->cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui->cont, 120, 0);
    lv_obj_set_style_bg_color(ui->cont, lv_color_hex(0x1C1C1E), 0);
    lv_obj_set_style_border_width(ui->cont, 0, 0);
    lv_obj_set_style_pad_all(ui->cont, 0, 0);

    /* ===== 可调参数 ===== */
    int digit_offset_x = -25; // 数码管X偏移
    int digit_offset_y = -35; // 数码管Y偏移

    int spacing = 30;         // 数码管间距
    int colon_gap = 5;        // 冒号HHMM之间间距
    int colon_w = 8;          // 冒号宽度
    int colon_spacing = 10;   // 冒号间距
    int colon_offset_y = 20;  // 冒号偏移Y
    int colon_offset_x = -7; // 冒号偏移X
    /* ===== 创建 digit ===== */
    for (int i = 0; i < 4; i++)
    {
        ui->digit[i] = lv_obj_create(ui->cont);
        lv_obj_remove_style_all(ui->digit[i]);

        for (int s = 0; s < 7; s++)
        {
            ui->seg[i][s] = lv_image_create(ui->digit[i]);
        }

        layout_digit(ui, i);
    }

    /* ===== 屏幕居中 ===== */
    int sw = lv_obj_get_width(parent);
    int sh = lv_obj_get_height(parent);

    int cx = sw / 2;
    int cy = sh / 2;

    int d_w = lv_obj_get_width(ui->digit[0]);
    int d_h = lv_obj_get_height(ui->digit[0]);

    int total_w = d_w * 4 + spacing * 3 + colon_w + colon_gap * 2;

    int start_x = cx - total_w / 2 + digit_offset_x;
    int base_y = cy - d_h / 2 + digit_offset_y;

    /* ===== HH ===== */
    int x = start_x;

    lv_obj_set_pos(ui->digit[0], x, base_y);
    x += d_w + spacing;

    lv_obj_set_pos(ui->digit[1], x, base_y);
    x += d_w;

    /* ===== 冒号（修复版） ===== */
    int colon_x = cx - colon_w / 2;
    int center_y = base_y + d_h / 2 + colon_offset_y;

    ui->colon[0] = lv_obj_create(ui->cont);
    ui->colon[1] = lv_obj_create(ui->cont);

    for (int i = 0; i < 2; i++)
    {
        lv_obj_t *c = ui->colon[i];

        lv_obj_set_size(c, colon_w, colon_w);
        lv_obj_set_style_radius(c, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(c, lv_color_hex(0xFFD700), 0);
        lv_obj_set_style_border_width(c, 0, 0);

        int y = center_y + (i == 0 ? -colon_spacing : colon_spacing);
        lv_obj_set_pos(c, colon_x + colon_offset_x, y);
    }

    /* ===== MM ===== */
    x = colon_x + colon_w / 2 + colon_gap;

    lv_obj_set_pos(ui->digit[2], x, base_y);
    x += d_w + spacing;

    lv_obj_set_pos(ui->digit[3], x, base_y);

    /* ===== timer ===== */
    ui->timer = lv_timer_create(timer_cb, 1000, ui);
    timer_cb(ui->timer);

    return ui;
}

/* ===== 删除 ===== */
void ui_digital_clock_delete(ui_digital_clock_t *ui)
{
    if (!ui)
        return;

    if (ui->timer)
        lv_timer_delete(ui->timer);
    if (ui->cont)
        lv_obj_delete(ui->cont);

    lv_free(ui);
}