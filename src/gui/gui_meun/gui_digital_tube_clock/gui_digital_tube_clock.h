#ifndef GUI_DIGITAL_TUBE_CLOCK_H
#define GUI_DIGITAL_TUBE_CLOCK_H    

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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../gui.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @breif 初始化菜单页面
     */
    void gui_digital_tube_clock_page(void);

    /**
     * @breif 删除菜单页面
     */
    void gui_digital_tube_clock_page_delete(void);


#ifdef __cplusplus
}
#endif

#endif /*GUI_DIGITAL_TUBE_CLOCK_H*/   
