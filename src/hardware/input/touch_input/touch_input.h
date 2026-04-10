#ifndef TOUCH_INPUT_H
#define TOUCH_INPUT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* 初始化 */
    void touch_input_init(void);

    /* 更新触摸点 */
    bool touch_input_update(uint16_t *x, uint16_t *y);

#ifdef __cplusplus
}
#endif

#endif /* TOUCH_INPUT_H */
