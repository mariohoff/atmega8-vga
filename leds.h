#ifndef LEDS_H
#define LEDS_H

void leds_init(void);
void leds_set(uint8_t val);
uint8_t leds_get(void);
void leds_update(void);

#endif
