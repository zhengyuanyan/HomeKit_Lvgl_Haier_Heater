#ifndef GUI_MOTOR_H
#define GUI_MOTOR_H


#include "../gui_system/gui_system.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @breif 初始化菜单页面
     */
    void gui_system_motor_page(lv_obj_t *parent);

    /**
     * @breif 异步运行菜单页面
     */
    void gui_system_motor_page_async_run(void *arg);
    /**
     * @breif 删除菜单页面
     */
    void gui_system_motor_page_delete(void);

    /**
     * @breif 异步运行菜单页面
     */

#ifdef __cplusplus
}
#endif

#endif /*GUI_MOTOR_H*/