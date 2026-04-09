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
     * 基础组件封装（带状态）
     * ========================= */

    // 开关组件
    typedef struct
    {
        ui_capsule_switch_t *ui;
        bool value;
        bool active;
    } gui_switch_t;

    // 旋钮组件
    typedef struct
    {
        ui_segment_knob_t *ui;
        uint8_t value;
        bool active;
    } gui_knob_t;

    // 亮度组件
    typedef struct
    {
        ui_brightness_t *ui;
        uint8_t value;
        bool active;
    } gui_brightness_t;

    // 温度组件
    typedef struct
    {
        ui_water_heater_t *ui;
        uint8_t temperature;
        bool active;
    } gui_temperature_t;

#define GUI_SCHEDULE_MAX 4

    typedef struct
    {
        ui_schedule_editor_t *ui;
        gui_knob_t main;
        schedule_t items[GUI_SCHEDULE_MAX]; 
        uint8_t active_index;
        bool active;
    } gui_schedule_t;

    /* =========================
     * 页面层（组合组件）
     * ========================= */

    // 主页
    typedef struct
    {
        gui_knob_t knob;
        bool active;
    } gui_main_t;

    // 加热页
    typedef struct
    {
        gui_switch_t heater;
        bool active;
    } gui_heater_t;

    // 温度页
    typedef struct
    {
        gui_temperature_t temperature;
        bool active;
    } gui_water_heater_t;

    // 系统页
    typedef struct
    {
        gui_knob_t main;
        gui_switch_t buzzer;
        gui_switch_t motor;
        gui_brightness_t display;
    } gui_system_t;

    /* =========================
     * 顶层 GUI（唯一入口）
     * ========================= */

    typedef struct
    {
        gui_main_t main;
        gui_heater_t heater;
        gui_water_heater_t water_heater;
        gui_schedule_t schedule;
        gui_system_t system;
    } gui_t;

    /* 全局唯一实例 */
    extern gui_t gui;

#ifdef __cplusplus
}
#endif

#endif /* GUI_TYPEDEF_H */