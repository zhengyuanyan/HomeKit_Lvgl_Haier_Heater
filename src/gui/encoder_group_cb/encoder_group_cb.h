#ifndef ENCODER_GROUP_CB_H
#define ENCODER_GROUP_CB_H

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

/**
 * @brief 编码器组事件回调函数
 * @param obj 编码器对象
 * @param event 事件
 * @param data 用户数据
 */
void encoder_add_focus_obj_group_event(lv_obj_t *obj, lv_event_cb_t cb, void *data);

/**
 * @brief 设置编码器对象编辑状态函数
 * @param obj 编码器对象
 */
void encoder_add_focus_obj_group_editing(lv_obj_t *obj, bool editing);

/**
 * @brief 设置编码器对象到编码器组函数
 * @param obj 编码器对象
 */
void encoder_add_focus_obj_group(lv_obj_t *obj);

/**
 * @brief 编码器设置编辑状态函数
 * @param editing 是否处于编辑状态
 */
void encoder_group_set_editing(bool editing);

/**
 * @brief 添加编码器对象到编码器组
 * @param obj 编码器对象
 */
void encoder_add_obj_group(lv_obj_t *obj);

/**
 * @brief 移除编码器对象从编码器组
 * @param obj 编码器对象
 */
void encoder_remove_obj_group(lv_obj_t *obj);

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif /*ENCODER_GROUP_CB_H*/