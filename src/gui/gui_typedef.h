#ifndef GUI_TYPEDEF_H
#define GUI_TYPEDEF_H

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

#include <stdint.h>
#include <stdbool.h>

#include "ui_lib/ui_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /* =========================
     * 通用组件封装（核心优化点）
     * ========================= */

    // 开关类组件
    typedef struct
    {
        ui_capsule_switch_t *ui;
        bool value;
    } gui_switch_t;

    // 旋钮类组件
    typedef struct
    {
        ui_segment_knob_t *ui;
        uint8_t value;
    } gui_knob_t;

    // 亮度类组件
    typedef struct
    {
        ui_brightness_t *ui;
        uint8_t value;
    } gui_brightness_t;

    // 温度类组件
    typedef struct
    {
        ui_water_heater_t *ui;
        uint8_t value;
    } gui_temperature_t;

    // 预约类组件
    typedef struct
    {
        ui_schedule_editor_t *ui;
        uint8_t value;
    } gui_schedule_t;
    /* =========================
     * 页面定义（只组合，不重复造轮子）
     * ========================= */

    // 菜单页
    typedef struct
    {
        gui_knob_t knob;
    } gui_menu_t;

    extern gui_menu_t gui_menu;

    // 加热开关页
    typedef struct
    {
        gui_switch_t heater;
    } gui_heater_t;

    extern gui_heater_t gui_heater_switch;

    // 温度页
    typedef struct
    {
        gui_temperature_t temperature;
    } gui_water_heater_t;

    extern gui_water_heater_t gui_water_heater;


    // 系统页
    typedef struct
    {
        gui_knob_t knob;
    } gui_system_t;

    extern gui_system_t gui_system;

    // 系统 - 蜂鸣器
    typedef struct
    {
        gui_switch_t buzzer;
    } gui_system_buzzer_t;

    extern gui_system_buzzer_t gui_system_buzzer;

    // 系统 - 马达
    typedef struct
    {
        gui_switch_t motor;
    } gui_system_motor_t;

    extern gui_system_motor_t gui_system_motor;

    // 系统 - 屏幕
    typedef struct
    {
        gui_brightness_t backlight;
    } gui_system_display_t;

    extern gui_system_display_t gui_system_display;

    /* =========================
     * 可选：统一GUI入口（强烈推荐）
     * ========================= */

    typedef struct
    {
        gui_menu_t menu;
        gui_heater_t heater;
        gui_system_t system;
        gui_system_buzzer_t buzzer;
        gui_system_motor_t motor;
        gui_system_display_t display;
    } gui_t;

    extern gui_t gui;

#ifdef __cplusplus
}
#endif

#endif /* GUI_TYPEDEF_H */