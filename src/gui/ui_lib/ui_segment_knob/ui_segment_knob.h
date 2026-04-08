#ifndef UI_SEGMENT_KNOB_H
#define UI_SEGMENT_KNOB_H

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

#define UI_SEGMENT_MAX_ITEMS 8

    typedef struct
    {
        const char *name;
        const void *icon;
        void (*enter_cb)(void *);
        void *user_data;

    } ui_segment_item_t;

    typedef struct
    {
        lv_obj_t *cont;

        lv_obj_t *arc[UI_SEGMENT_MAX_ITEMS];
        lv_obj_t *icon[UI_SEGMENT_MAX_ITEMS];

        lv_obj_t *center_label;

        lv_color_t item_color[12];

        ui_segment_item_t items[UI_SEGMENT_MAX_ITEMS];

        uint8_t item_count;
        int current_index;

    } ui_segment_knob_t;

    ui_segment_knob_t *ui_segment_knob_create(lv_obj_t *parent);

    void ui_segment_knob_add_item(ui_segment_knob_t *ui, const char *name, const void *icon, void (*enter_cb)(void *), void *user_data);
    void ui_segment_knob_delete(ui_segment_knob_t *ui);
    void ui_segment_knob_clear(ui_segment_knob_t *ui);
    void ui_segment_knob_next(ui_segment_knob_t *ui);
    void ui_segment_knob_prev(ui_segment_knob_t *ui);
    void ui_segment_knob_enter(ui_segment_knob_t *ui);

#ifdef __cplusplus
}
#endif

#endif /* UI_SEGMENT_KNOB_H */