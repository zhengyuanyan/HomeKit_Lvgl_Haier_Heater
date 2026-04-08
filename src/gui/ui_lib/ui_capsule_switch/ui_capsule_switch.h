#ifndef UI_CAPSULE_SWITCH_H
#define UI_CAPSULE_SWITCH_H

#ifdef __cplusplus
extern "C"
{
#endif

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

    typedef struct
    {
        lv_obj_t *cont;
        lv_obj_t *label_off;
        lv_obj_t *label_on;
        lv_obj_t *slider;

        bool state;
        uint8_t focus;
    } ui_capsule_switch_t;

    ui_capsule_switch_t *ui_capsule_switch_create(lv_obj_t *parent);

    void ui_capsule_switch_delete(ui_capsule_switch_t *ui);

    void ui_capsule_switch_left(ui_capsule_switch_t *ui);

    void ui_capsule_switch_right(ui_capsule_switch_t *ui);

    void ui_capsule_switch_enter(ui_capsule_switch_t *ui);

    void ui_capsule_switch_set(ui_capsule_switch_t *ui, bool on);

    bool ui_capsule_switch_get(ui_capsule_switch_t *ui);


#ifdef __cplusplus
}
#endif

#endif /* UI_CAPSULE_SWITCH_H */