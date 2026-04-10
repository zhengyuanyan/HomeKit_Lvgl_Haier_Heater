#include "touch_input.h"
#include <Wire.h>
#include "TouchDrvFT6X36.hpp"
#include "esp_log.h"
#include "../../i2c_manager/i2c_manager.h"

static const char *TAG = "Touch";
static TouchDrvFT6X36 touch;

/* 初始化 */
void touch_input_init(void)
{
    i2c_init();

    if (!touch.begin(Wire, FT6X36_SLAVE_ADDRESS))
    {
        ESP_LOGE(TAG, "FT6X36 init failed\n");
        while (1)
            ;
    }

    touch.interruptPolling();
}

/* 外部周期调用（关键） */
bool touch_input_update(uint16_t *x, uint16_t *y)
{
    TouchPoints points = touch.getTouchPoints();

    if (points.hasPoints())
    {
        const TouchPoint &p = points.getPoint(0);

        *x = p.x;
        *y = p.y;
        return true;
    }
    return false;
}
