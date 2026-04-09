#ifndef GUI_MEUN_H
#define GUI_MEUN_H

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
    void gui_main_page(void);

    /**
     * @breif 删除菜单页面
     */
    void gui_main_page_delete(void);
    /**
     * @breif 删除菜单页面所有子页面
     */
    void gui_main_page_subpage_delete_all(void);

#ifdef __cplusplus
}
#endif

#endif /*GUI_MEUN_H*/