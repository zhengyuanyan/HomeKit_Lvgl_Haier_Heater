#ifndef LV_UI_IMAGES_H
#define LV_UI_IMAGES_H

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

    LV_IMAGE_DECLARE(Water_heater_arc);
    LV_IMAGE_DECLARE(Color_temperature_arc);
    LV_IMAGE_DECLARE(Brightness_arc);
    LV_IMAGE_DECLARE(image_color_wheel);

    LV_IMAGE_DECLARE(digital_tube_a);
    LV_IMAGE_DECLARE(digital_tube_b);
    LV_IMAGE_DECLARE(digital_tube_c);
    LV_IMAGE_DECLARE(digital_tube_d);
    LV_IMAGE_DECLARE(digital_tube_e);
    LV_IMAGE_DECLARE(digital_tube_f);
    LV_IMAGE_DECLARE(digital_tube_g);

    LV_IMAGE_DECLARE(digital_tube_h_a);
    LV_IMAGE_DECLARE(digital_tube_h_b);
    LV_IMAGE_DECLARE(digital_tube_h_c);
    LV_IMAGE_DECLARE(digital_tube_h_d);
    LV_IMAGE_DECLARE(digital_tube_h_e);
    LV_IMAGE_DECLARE(digital_tube_h_f);
    LV_IMAGE_DECLARE(digital_tube_h_g);
#ifdef __cplusplus
}
#endif

#endif /*LV_UI_IMAGES_H*/