#include <Arduino.h>
#include "RMT_WS2812.hpp"
#include "Adafruit_NeoPixel.h"

#define CONFIG_RMT_CHANNEL                  RMT_CHANNEL_0
#define CONFIG_MAX_BRIGHTNESS               20
#define CONFIG_SLEEP_MS                     15

// #if defined(ARDUINO_M5Stack_ATOM)
// #define CONFIG_RMT_GPIO                     GPIO_NUM_27
// #define CONFIG_LED_NUM                      25
// #elif defined(ARDUINO_STAMP_C3)
// #define CONFIG_RMT_GPIO                     GPIO_NUM_2
// #define CONFIG_LED_NUM                      1
// #endif

#define CONFIG_RMT_GPIO GPIO_NUM_15
#define CONFIG_LED_NUM 300

// RMT_WS2812 led(CONFIG_RMT_CHANNEL, CONFIG_RMT_GPIO, CONFIG_LED_NUM);
Adafruit_NeoPixel led = Adafruit_NeoPixel(CONFIG_LED_NUM, CONFIG_RMT_GPIO, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
	Serial.println("Hello World");
  delay(1000);

  led.begin();
  led.clear();
}

int nnn=0;

void loop() {
	if(Serial.available()){
		char c = Serial.read();
		Serial.println(c);
	}

	nnn++;
	if(nnn>255) nnn=0;
	for(int i=0; i<CONFIG_LED_NUM; i++){
		// led.setPixel(i, 30, 30, 30);
		led.setPixelColor(i, nnn, 0, 0);
	}
	// led.refresh();
	led.show();

  // static int8_t brightness = 0;
  // static int8_t color = 0;
  // brightness--;
  // if (brightness < 0) {
  //   brightness = CONFIG_MAX_BRIGHTNESS;
  //   color--;
  //   if (color < 0) {
  //     color = 7;
  //   }
  //   for (int i = 0; i < CONFIG_LED_NUM; i++) {
  //     uint8_t red   = (color & 1) ? 255 : 0;
  //     uint8_t green = (color & 2) ? 255 : 0;
  //     uint8_t blue  = (color & 4) ? 255 : 0;
  //     led.setPixel(i, red, green, blue);
  //   }
  // }

  // led.setBrightness(brightness);
  // led.refresh();

  // delay(CONFIG_SLEEP_MS);
}