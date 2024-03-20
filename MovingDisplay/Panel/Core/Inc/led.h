#pragma once
#include "main.h"
#include <stdint.h>
#include "ws2812b.h"

class LED{
    private:
    WS2812B* NEOPIXEL;
    uint8_t red,green,blue;
    #define PANEL_START_X 0
    #define PANEL_START_Z 0

    public:
    LED(WS2812B* neopixel);
    void init();
    int change_coord_to_num(int px, int pz);
    void execute_NPX_execute();
    void set_under();
    void set_circle(int x, int z, int cx, int cz);
    void set_color(uint16_t pixel_num);
    void show(int travel_x, int circle_x, int circle_z);
};
