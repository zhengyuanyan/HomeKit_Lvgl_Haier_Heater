#ifndef MSG_LVGL_H
#define MSG_LVGL_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // ================= 输入类型 =================
    // typedef enum
    // {
    //     INPUT_TYPE_NONE = 0,

    //     INPUT_TYPE_ENCODER, // 旋转
    //     INPUT_TYPE_BUTTON,  // 按键
    //     INPUT_TYPE_TOUCH,   // 触摸（预留）
    // } lvgl_type_t;

    // // ================= 具体事件 =================
    // typedef enum
    // {
    //     INPUT_ACTION_ROTATE = 0,
    //     INPUT_ACTION_SHORT,
    //     INPUT_ACTION_LONG,
    // } lvgl_action_t;

    // // ================= 通用消息 =================
    // typedef struct
    // {
    //     lvgl_type_t type;
    //     lvgl_action_t action;

    //     union
    //     {
    //         struct
    //         {
    //             int diff_value; // encoder diff
    //             uint8_t key;      // button
    //         } encoder;

    //         struct  
    //         {
    //             bool touch; 
    //             int16_t x;
    //             int16_t y;
    //         } touch;
    //     } data;

    //     uint32_t timestamp;

    // } lvgl_msg_t;



#ifdef __cplusplus
}
#endif

#endif /* MSG_LVGL_H */
