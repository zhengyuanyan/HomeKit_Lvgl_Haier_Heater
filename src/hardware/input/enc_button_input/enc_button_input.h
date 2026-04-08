#ifndef ENC_BUTTON_INPUT_H
#define ENC_BUTTON_INPUT_H

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

#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void encoder_button_init(void);
    uint8_t encoder_button_read(void);

#ifdef __cplusplus
}
#endif

#endif
