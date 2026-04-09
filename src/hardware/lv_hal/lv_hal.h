#ifndef LV_HAL_H
#define LV_HAL_H

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
#include "driver/gpio.h"
#include "../encoder/encoder.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup 硬件初始化
     */
    void lv_hal_init(void);

    /**
     *  @brief 设置TFT亮度
     * @param brightness 亮度值
     */
    void tft_set_brightness(uint8_t brightness);

    /**
     * @brief 获取编码器组对象
     * @return 编码器组对象
     */
    lv_group_t *get_lv_group(void);

    void lv_hal_set_encoder_group(lv_group_t *group);

#ifdef __cplusplus
}
#endif

#endif /* LV_HAL_H */