#ifndef LV_LOCK_H
#define LV_LOCK_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

    esp_err_t lvgl_lock_init(void);
    esp_err_t lvgl_lock(void);
    esp_err_t lvgl_unlock(void);

#ifdef __cplusplus
}
#endif

#endif /* LV_LOCK_H */
