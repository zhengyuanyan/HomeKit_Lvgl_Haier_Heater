#ifndef TASK_H
#define TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void task_init(void);
    
#ifdef __cplusplus
}
#endif

#endif /* TASK_H */