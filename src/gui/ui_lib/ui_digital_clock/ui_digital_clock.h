#ifndef UI_DIGITAL_CLOCK_H
#define UI_DIGITAL_CLOCK_H

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

#include <stdbool.h>

/* ===== 参数 ===== */
#define DIGIT_COUNT 4 // 数码管数量
#define SEG_COUNT 7   // 段数量
#define COLON_COUNT 2 // 冒号数量


    /* ===== 数码管结构 ===== */
    typedef struct
    {
        lv_obj_t *cont;                        // 数码管容器
        lv_obj_t *digit[DIGIT_COUNT];          // 数码管
        lv_obj_t *seg[DIGIT_COUNT][SEG_COUNT]; // 段
        lv_obj_t *colon[COLON_COUNT];          // 冒号
        lv_timer_t *timer;                     // 定时器
        bool colon_on;                         // 冒号是否点亮

    } ui_digital_clock_t;

    /* ===== API ===== */

    /**
     * @brief 创建数字时钟
     */
    ui_digital_clock_t *ui_digital_clock_create(lv_obj_t *parent);

    /**
     * @brief 删除数字时钟
     */
    void ui_digital_clock_delete(ui_digital_clock_t *ui);

    /**
     * @brief 设置一个定时任务
     *
     * @param ui      时钟对象
     * @param id      任务ID（0~MAX_ALARMS-1）
     * @param hour    小时（0~23）
     * @param min     分钟（0~59）
     * @param cb      回调函数（触发时执行）
     */
    void ui_digital_clock_set_alarm(ui_digital_clock_t *ui, int id, int hour, int min, void (*cb)(void));

    /**
     * @brief 关闭某个定时任务
     */
    void ui_digital_clock_disable_alarm(ui_digital_clock_t *ui, int id);

#ifdef __cplusplus
}
#endif

#endif
