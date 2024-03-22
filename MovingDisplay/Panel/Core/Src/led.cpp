#include "led.h"

LED::LED(WS2812* neopixel){
    NEOPIXEL = neopixel;
}

void LED::init(uint8_t panel_id){
    NEOPIXEL->clear();
    if	   (panel_id==0){PANEL_START_X = 0; PANEL_START_Z = 0;}
    else if(panel_id==1){PANEL_START_X = 0; PANEL_START_Z = 16;}
    else if(panel_id==2){PANEL_START_X = 0; PANEL_START_Z = 32;}

    else if(panel_id==3){PANEL_START_X = 16; PANEL_START_Z = 0;}
    else if(panel_id==4){PANEL_START_X = 16; PANEL_START_Z = 16;}
    else if(panel_id==5){PANEL_START_X = 16; PANEL_START_Z = 32;}

    else if(panel_id==6){PANEL_START_X = 32; PANEL_START_Z = 0;}
    else if(panel_id==7){PANEL_START_X = 32; PANEL_START_Z = 16;}
    else if(panel_id==8){PANEL_START_X = 32; PANEL_START_Z = 32;}
}

void LED::show(int travel_x, int circle_x, int circle_z, int circle_r, int hue, int hue_of_back){
    NEOPIXEL->clear();
    for(int px=0; px<16; px++){
        for(int pz=0; pz<16; pz++){
            int x = px + PANEL_START_X + travel_x;
            int z = pz + PANEL_START_Z;

            set_under(hue_of_back);
            set_circle(x, z, circle_x, circle_z, circle_r, hue);
            set_color(change_coord_to_num(px, pz));
        }
    }
    NEOPIXEL->show();
}

void LED::set_circle(int x, int z, int cx, int cz, int ro, int h){
    float distance = (x-cx)*(x-cx)+(z-cz)*(z-cz);
    if(ro*ro>=distance){
//        red = 10; green = 0; blue = 0;
    	hue = h; sat = 240; val = 20;
    }else{}
}

void LED::do_forwardRewrite(){
    NEOPIXEL->do_forwardRewrite();
}

void LED::do_backRewrite(){
    NEOPIXEL->do_backRewrite();
}

void LED::set_under(int hue_of_back){
    hue = hue_of_back;
    sat = 230;
    val = 8;
}

void LED::set_color(uint16_t pixel_num){
//    NEOPIXEL->set_rgb(pixel_num, red, green, blue);
	NEOPIXEL->set_hsv(pixel_num, hue, sat, val);
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
