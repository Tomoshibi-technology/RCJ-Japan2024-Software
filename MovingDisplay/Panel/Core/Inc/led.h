#pragma once
#include "main.h"
#include <stdint.h>
#include "ws2812.h"
#include <math.h>

class LED{
    private:
    WS2812* NEOPIXEL;
    uint8_t red,green,blue;
    uint8_t PANEL_START_X;
    uint8_t PANEL_START_Z;
    int hue, sat, val;

    public:
    LED(WS2812* neopixel);
    void init(uint8_t panel_id);
    void do_forwardRewrite();
    void do_backRewrite();
    int change_coord_to_num(int px, int pz);;
    void set_under(int hue_of_back, int v);
    void set_circle(int x, int z, int cx, int cz, int ro, int h, int v);
    void set_color(uint16_t pixel_num);
    void show(int travel_x, int circle_x, int circle_z, int circle_r, int hue, int hue_of_back, int v);
};
