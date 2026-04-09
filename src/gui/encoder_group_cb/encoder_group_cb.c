#include "encoder_group_cb.h"
#include "../../hardware/lv_hal/lv_hal.h"

void encoder_add_obj_group(lv_obj_t *obj)
{
    if (!obj)
        return;
    lv_group_t *lvGroup = get_lv_group();
    if (!lvGroup)
        return;

    lv_group_add_obj(lvGroup, obj);
}

void encoder_remove_obj_group(lv_obj_t *obj)
{
    if (!obj)
        return;
    lv_group_t *lvGroup = get_lv_group();
    if (!lvGroup)
        return;

    lv_group_remove_obj(obj);
}

void encoder_focus_obj_group(lv_obj_t *obj)
{
    if (!obj)
        return;
    lv_group_focus_obj(obj);
}

void encoder_group_set_editing(bool editing)
{
    lv_group_t *lvGroup = get_lv_group();
    if (!lvGroup)
        return;
    lv_group_set_editing(lvGroup, editing);
}

void encoder_add_focus_obj_group_editing(lv_obj_t *obj, bool editing)
{
    if (!obj)
        return;
    lv_group_t *lvGroup = get_lv_group();
    if (!lvGroup)
        return;

    lv_group_add_obj(lvGroup, obj);
    lv_group_focus_obj(obj);
    lv_group_set_editing(lvGroup, editing);
}

void encoder_add_focus_obj_group_event(lv_obj_t *obj, lv_event_cb_t cb, void *data)
{
    if (!obj)
        return;
    lv_group_t *lvGroup = get_lv_group();
    if (!lvGroup)
        return;

    lv_group_add_obj(lvGroup, obj);
    lv_group_focus_obj(obj);
    lv_group_set_editing(lvGroup, true);
    if (cb)
        lv_obj_add_event_cb(obj, cb, LV_EVENT_ALL, data);
}
void encoder_add_focus_obj_group(lv_obj_t *obj)
{
    encoder_add_focus_obj_group_event(obj, NULL, NULL);
}

void encoder_set_group(lv_group_t *group)
{

    if (!group)
        return;
    group = get_lv_group();
}