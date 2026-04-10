#include "msg_input.h"
#include "msg_lvgl.h"



#define INPUT_MAX_SUBSCRIBERS 4
#define INPUT_MSG_QUEUE_LEN 20

static QueueHandle_t input_subscriber_queues[INPUT_MAX_SUBSCRIBERS];
static int input_subscriber_count = 0;

void input_broadcast_init(void)
{
    for (int i = 0; i < INPUT_MAX_SUBSCRIBERS; i++)
    {
        input_subscriber_queues[i] = NULL;
    }
    input_subscriber_count = 0;
}

int input_broadcast_subscribe(void)
{
    if (input_subscriber_count >= INPUT_MAX_SUBSCRIBERS)
    {
        return -1;
    }

    input_subscriber_queues[input_subscriber_count] = xQueueCreate(INPUT_MSG_QUEUE_LEN, sizeof(input_msg_t));
    if (!input_subscriber_queues[input_subscriber_count])
        return -1;
    return input_subscriber_count++;
}

// 广播消息给所有订阅者
void input_broadcast_send(input_msg_t msg)
{
    for (int i = 0; i < input_subscriber_count; i++)
    {
        if (input_subscriber_queues[i])
        {
            xQueueSend(input_subscriber_queues[i], &msg, 0); // 非阻塞
        }
    }
}

// 从订阅者队列中获取消息
bool input_broadcast_pop(int sub_id, input_msg_t *msg, TickType_t timeout)
{
    if (sub_id < 0 || sub_id >= input_subscriber_count)
    {
        return false;
    }
    if (!input_subscriber_queues[sub_id])
    {
        return false;
    }
    return xQueueReceive(input_subscriber_queues[sub_id], msg, timeout) == pdTRUE;
}
