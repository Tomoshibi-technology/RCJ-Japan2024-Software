#pragma once

#include <Arduino.h>
#include "driver/rmt.h"

extern uint32_t  ws2812_t0h_ticks;
extern uint32_t  ws2812_t1h_ticks;
extern uint32_t  ws2812_t0l_ticks;
extern uint32_t  ws2812_t1l_ticks;
extern uint8_t   ws2812_brightness;
extern void IRAM_ATTR ws2812_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num);

class RMT_WS2812 {
  public:
    RMT_WS2812(rmt_channel_t channel, gpio_num_t gpip, uint16_t ledNum) {
      _rmtChannel = channel;
      _rmtGpio = gpip;
      _ledNum = ledNum;
    };

    void begin(void) {
      rmt_config_t rmtConf = RMT_DEFAULT_CONFIG_TX(_rmtGpio, _rmtChannel);
      rmtConf.clk_div = 2; // Set 40MHz

      ESP_ERROR_CHECK(rmt_config(&rmtConf));
      ESP_ERROR_CHECK(rmt_driver_install(rmtConf.channel, 0, 0));

      uint32_t buffSize = _ledNum * 3;
      _buffer = (uint8_t*)calloc(1, buffSize);
      if (_buffer == NULL) {
        ESP_LOGE(TAG, "%s(%d): " "calloc failed", __FUNCTION__, __LINE__);
        return;
      }

      uint32_t counter_clk_hz = 0;
      if (rmt_get_counter_clock(_rmtChannel, &counter_clk_hz) != ESP_OK) {
        ESP_LOGE(TAG, "%s(%d): " "rmt_get_counter_clock failed", __FUNCTION__, __LINE__);
        return;
      }

      // ns -> ticks
      float ratio = (float)counter_clk_hz / 1e9;
      ws2812_t0h_ticks = (uint32_t)(ratio * WS2812_T0H_NS);
      ws2812_t0l_ticks = (uint32_t)(ratio * WS2812_T0L_NS);
      ws2812_t1h_ticks = (uint32_t)(ratio * WS2812_T1H_NS);
      ws2812_t1l_ticks = (uint32_t)(ratio * WS2812_T1L_NS);

      // set ws2812 to rmt adapter
      rmt_translator_init(_rmtChannel, ws2812_rmt_adapter);

      ESP_LOGV(TAG, "%s(%d): " "RMT_WS2812 begin channel(%d), gpip(%d), ledNum(%d)", __FUNCTION__, __LINE__, _rmtChannel, _rmtGpio, _ledNum);
    };

    esp_err_t clear(uint32_t timeout_ms = 200) {
      memset(_buffer, 0, _ledNum * 3);
      return refresh(timeout_ms);
    }

    esp_err_t refresh(uint32_t timeout_ms = 200) {
      if (rmt_write_sample(_rmtChannel, _buffer, _ledNum * 3, true) != ESP_OK) {
        ESP_LOGE(TAG, "%s(%d): " "rmt_write_sample failed", __FUNCTION__, __LINE__);
        return ESP_FAIL;
      }

      return rmt_wait_tx_done(_rmtChannel, pdMS_TO_TICKS(timeout_ms));
    }

    esp_err_t setPixel(uint32_t index, uint8_t red, uint8_t green, uint8_t blue) {
      if (_ledNum < index) {
        ESP_LOGE(TAG, "%s(%d): " "index(%d) is out of range(%d)", __FUNCTION__, __LINE__, index, _ledNum);
        return ESP_ERR_INVALID_ARG;
      }

      uint32_t start = index * 3;
      _buffer[start + 0] = green & 0xFF;
      _buffer[start + 1] = red & 0xFF;
      _buffer[start + 2] = blue & 0xFF;
      return ESP_OK;
    }

		void setPixel_hsv(uint32_t index, uint8_t hue, uint8_t saturation, uint8_t setBrightness){
			int r, g, b;
			int h = hue;
			int s = saturation;
			int v = setBrightness;

			if (s == 0) {
				r = g = b = v;
			}else{
				h = h * 6; // sector 0 to 5
				int i = h / 256;
				int f = h % 256;
				int p = (v * (256 - s)) / 256;
				int q = (v * (256 - (s * f) / 256)) / 256;
				int t = (v * (256 - (s * (256 - f)) / 256)) / 256;

				switch (i) {
					case 0: r = v; g = t; b = p; break;
					case 1: r = q; g = v; b = p; break;
					case 2: r = p; g = v; b = t; break;
					case 3: r = p; g = q; b = v; break;
					case 4: r = t; g = p; b = v; break;
					default: r = v; g = p;b = q; break;
				}
			}
			// Serial.print("R" + String(r) +String(" g") + String(g) + String(" b") + String(b) + String(" "));
			setPixel(index, r, g, b);
		}

    void setBrightness(uint8_t brightness) {      ws2812_brightness = brightness % 100;
    }

    const uint16_t WS2812_T0H_NS    =  350;
    const uint16_t WS2812_T0L_NS    = 1000;
    const uint16_t WS2812_T1H_NS    = 1000;
    const uint16_t WS2812_T1L_NS    =  350;
    const uint16_t WS2812_RESET_US  =  280;

  private:
    rmt_channel_t _rmtChannel;
    gpio_num_t _rmtGpio;
    uint32_t _ledNum;
    uint8_t *_buffer;
    const char *TAG = "RMT_WS2812";
};