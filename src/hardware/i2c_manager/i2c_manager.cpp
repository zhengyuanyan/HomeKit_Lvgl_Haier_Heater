#include "i2c_manager.h"

#include <Wire.h>

void i2c_init(void)
{
    static bool inited = false;

    if (!inited)
    {
        Wire.begin(i2c_sda_Pin, i2c_scl_Pin, i2c_clock_Speed);
        inited = true;
    }
}
