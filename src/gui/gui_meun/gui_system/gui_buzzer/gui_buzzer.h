#ifndef GUI_BUZZER_H
#define GUI_BUZZER_H


#include "../gui_system/gui_system.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @breif 初始化菜单页面
     */
    void gui_system_buzzer_page(lv_obj_t *parent);

    /**
     * @breif 异步运行菜单页面
     */
    void gui_system_buzzer_page_async_run(void *arg);
    /**
     * @breif 删除菜单页面
     */
    void gui_system_buzzer_page_delete(void);

    /**
     * @breif 异步运行菜单页面
     */

#ifdef __cplusplus
}
#endif

#endif /*GUI_SYSTEM_H*/