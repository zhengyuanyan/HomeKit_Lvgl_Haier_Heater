#ifndef UI_COLOR_WHEEL_H
#define UI_COLOR_WHEEL_H

#ifdef __has_include
#if __has_include("lvgl.h")
#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#define LV_LVGL_H_INCLUDE_SIMPLE
#endif
#endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    /* RGB回调 */

    typedef enum
    {
        WHEEL_MODE_COLOR,
        WHEEL_MODE_BRIGHTNESS
    } wheel_mode_t;

    typedef struct
    {
        lv_obj_t *cont;

        lv_obj_t *wheel;
        lv_obj_t *indicator;
        lv_obj_t *brightness_arc;

        uint8_t r;
        uint8_t g;
        uint8_t b;

        uint8_t brightness;
        float last_angle;         // 上一次处理的角度（弧度）
        bool brightness_dragging; // 是否正在拖动亮度
        uint8_t start_brightness;

        wheel_mode_t mode;
        float hue;
    } ui_color_wheel_t;

    ui_color_wheel_t *ui_color_wheel_create(lv_obj_t *parent, const lv_img_dsc_t *wheel_img);
    void ui_color_wheel_delete(ui_color_wheel_t *ui);

    void ui_color_wheel_set_color(ui_color_wheel_t *ui, uint8_t r, uint8_t g, uint8_t b);
    void ui_color_wheel_get_color(ui_color_wheel_t *ui, uint8_t *r, uint8_t *g, uint8_t *b);
    void ui_color_wheel_set_brightness(ui_color_wheel_t *ui, uint8_t brightness);
    uint8_t ui_color_wheel_get_brightness(ui_color_wheel_t *ui);

    void ui_color_wheel_encoder(ui_color_wheel_t *ui, int diff);
    void ui_color_wheel_encoder_enter(ui_color_wheel_t *ui);

#ifdef __cplusplus
}
#endif

#endif /*UI_COLOR_WHEEL_H*/