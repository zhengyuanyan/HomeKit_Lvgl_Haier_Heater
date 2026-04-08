#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#define WIDTH 240
#define HEIGHT 240

class LGFX : public lgfx::LGFX_Device
{

    lgfx::Panel_GC9A01 _panel_instance;

    lgfx::Bus_SPI _bus_instance;

    lgfx::Light_PWM _light_instance;

    // lgfx::Touch_FT5x06 _touch_instance;

public:
    LGFX(void)
    {
        // --- SPI bus ---
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host = SPI3_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000; // 40 MHz write clock
            cfg.freq_read = 16000000;  // 16 MHz read clock
            cfg.spi_3wire = true;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            cfg.pin_sclk = GPIO_NUM_13;
            cfg.pin_mosi = GPIO_NUM_11;
            cfg.pin_miso = (gpio_num_t)-1;
            cfg.pin_dc = GPIO_NUM_14;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {                                        // Set display panel control.
            auto cfg = _panel_instance.config(); // Get the structure for display panel settings.

            cfg.pin_cs = GPIO_NUM_10;   // Pin number to which CS is connected (-1 = disable)
            cfg.pin_rst = GPIO_NUM_15;  // pin number where RST is connected (-1 = disable)
            cfg.pin_busy = (gpio_num_t)-1; // pin number to which BUSY is connected (-1 = disable)

            // * The following setting values ​​are set to general default values ​​for each panel, and the pin number (-1 = disable) to which BUSY is connected, so please try commenting out any unknown items.

            // cfg.memory_width = WIDTH;  // Maximum width supported by driver IC
            // cfg.memory_height = HEIGHT; // Maximum height supported by driver IC
            cfg.panel_width = WIDTH;   // actual displayable width
            cfg.panel_height = HEIGHT;  // actual displayable height
            cfg.offset_x = 0;        // Panel offset in X direction
            cfg.offset_y = 0;        // Panel offset in Y directioncfg.offset_rotation = 2;

            cfg.readable = false;
            cfg.invert = true;


            _panel_instance.config(cfg);
        }
        {                                        // Set backlight control. (delete if not necessary)
            auto cfg = _light_instance.config(); // Get the structure for backlight configuration.

            cfg.pin_bl = GPIO_NUM_16;     // pin number to which the backlight is connected
            cfg.invert = false;  // true to invert backlight brightness
            cfg.freq = 44100;    // backlight PWM frequency
            cfg.pwm_channel = 0; // PWM channel number to use

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
        }

        // { // Configure settings for touch screen control. (delete if not necessary)
        //     auto cfg = _touch_instance.config();

        //     cfg.x_min = 0;    // Minimum X value (raw value) obtained from the touchscreen
        //     cfg.x_max = WIDTH - 1;  // Maximum X value (raw value) obtained from the touchscreen
        //     cfg.y_min = 0;    // Minimum Y value obtained from touchscreen (raw value)
        //     cfg.y_max = HEIGHT - 1;  // Maximum Y value (raw value) obtained from the touchscreen
        //     cfg.pin_int = 14; // pin number to which INT is connected
        //     cfg.bus_shared = true;
        //     cfg.offset_rotation = 0;

        //     // For I2C connection
        //     cfg.i2c_port = 0;    // Select I2C to use (0 or 1)
        //     cfg.i2c_addr = 0x38; // I2C device address number
        //     cfg.pin_sda = GPIO_NUM_11;    // pin number where SDA is connected
        //     cfg.pin_scl = GPIO_NUM_12;    // pin number to which SCL is connected
        //     cfg.freq = 400000;   // set I2C clock

        //     _touch_instance.config(cfg);
        //     _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
        // }

        setPanel(&_panel_instance); // Sets the panel to use.
    }
};

LGFX tft;
