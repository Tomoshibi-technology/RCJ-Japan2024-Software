#include "RMT_WS2812.hpp"

uint32_t  ws2812_t0h_ticks;
uint32_t  ws2812_t1h_ticks;
uint32_t  ws2812_t0l_ticks;
uint32_t  ws2812_t1l_ticks;
uint8_t   ws2812_brightness = 20; // 0-100

void IRAM_ATTR ws2812_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num) {
  if (src == NULL || dest == NULL) {
    *translated_size = 0;
    *item_num = 0;
    return;
  }
  const rmt_item32_t bit0 = {ws2812_t0h_ticks, 1, ws2812_t0l_ticks, 0};
  const rmt_item32_t bit1 = {ws2812_t1h_ticks, 1, ws2812_t1l_ticks, 0};
  size_t size = 0;
  size_t num = 0;
  uint8_t *psrc = (uint8_t *)src;
  rmt_item32_t *pdest = dest;
  while (size < src_size && num < wanted_num) {
    for (int i = 0; i < 8; i++) {
      if ((*psrc * ws2812_brightness / 100) & (1 << (7 - i))) {
        pdest->val =  bit1.val;
      } else {
        pdest->val =  bit0.val;
      }
      num++;
      pdest++;
    }
    size++;
    psrc++;
  }
  *translated_size = size;
  *item_num = num;
}