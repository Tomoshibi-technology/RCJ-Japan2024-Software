#include "led.h"

LED::LED(WS2812B* neopixel){
    NEOPIXEL = neopixel;
}

void LED::init(){
    NEOPIXEL->init();
}

void LED::show(int travel_x, int circle_x, int circle_z){
    for(int px=0; px<16; px++){
        for(int pz=0; pz<16; pz++){
            int x = PANEL_START_X + travel_x;
            int z = PANEL_START_Z;

            set_under();
            // set_circle(x, z, circle_x, circle_z);
            set_color(change_coord_to_num(px, pz));
        }
    }
    NEOPIXEL->show();
}

void LED::execute_NPX_execute(){
    NEOPIXEL->execute();
}

void LED::set_under(){
    red = 0;
    blue = 3;
    green = 1;
}

void LED::set_color(uint16_t pixel_num){
    NEOPIXEL->set_color(pixel_num, red, blue, green);
}

int LED::change_coord_to_num(int px, int pz){
    int return_pixel_num = 0;

    if(pz%2 == 0){
        return_pixel_num = pz*16 + px;
    }
    else{
        return_pixel_num = pz*16 + 15 - px;
    }

    return return_pixel_num;
}