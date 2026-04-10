#include "i2c_manager.h"
#include <Wire.h>

void i2c_init(void)
{
    static bool inited = false;

    if (!inited)
    {
        Wire.setPins(I2C_SDA_Pin, I2C_SCL_Pin);  
        
        Wire.begin();

        Wire.setClock(I2C_CLOCK_Speed);

        inited = true;
    }
}
