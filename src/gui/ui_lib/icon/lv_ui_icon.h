#ifndef LV_UI_ICON_H
#define LV_UI_ICON_H

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

    LV_FONT_DECLARE(ui_segment_knob_icon_24); // 0xe638,0xe602,0xe61c,0xe606,0xe601,0xe615,0xe8c1,0xe61e,0xe603,0xe63f,0xe612,0xe639,0xe629,0xe624,0xe722

#define LV_MENU_SYMBOL_TEMPERATURE "\xEE\x98\x86" // Unicode  E606
#define LV_MENU_SYMBOL_HEATING "\xEE\x98\x92"     // Unicode  E612
#define LV_MENU_SYMBOL_TIMING "\xEE\x98\x9E"      // Unicode  E61E
#define LV_MENU_SYMBOL_SETTINGS "\xEE\x98\x95"    // Unicode  E620

#define LV_SETUP_SYMBOL_CHIP "\xEE\x98\x83"       // Unicode  E603
#define LV_SETUP_SYMBOL_WIFI "\xEE\xA3\x81"       // Unicode  E8C1
#define LV_SETUP_SYMBOL_HOMEKIT "\xEE\x98\xA4"    // Unicode  E624
#define LV_SETUP_SYMBOL_TIMING "\xEE\x98\x82"     // Unicode  E602
#define LV_SETUP_SYMBOL_BUZZER "\xEE\x98\xB8"     // Unicode  E638
#define LV_SETUP_SYMBOL_MOTOR "\xEE\x98\x81"      // Unicode  E601
#define LV_SETUP_SYMBOL_BRIGHTNESS "\xEE\x9C\xA2" // Unicode  E7C2
#define LV_SETUP_SYMBOL_ENTER "\xEE\x99\xB9"      // Unicode  E639
#define LV_SETUP_SYMBOL_ESC "\xEE\x98\x9C"        // Unicode  E61C
#define LV_SETUP_SYMBOL_WS2812 "\xEE\x98\xBF"     // Unicode  E63F

#ifdef __cplusplus
}
#endif

#endif /*LV_UI_ICON_H*/