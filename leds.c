
#include <avr/io.h>
#include "leds.h"

static uint8_t led_register;

void leds_init(void) {
        //Ausgaenge LED
        DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4);
        DDRB |= (1 << PB3) | (1 << PB4) | (1 << PB5);
}

//setze LEDs auf value zwischen 0 und 255
void leds_set(uint8_t val)
{
        led_register = 0x00;

        //flip bits
        for(uint8_t i = 0; i < 8; ++i) {
                if(((val >> i) & 1))
                        led_register |= (1 << (7-i));
        }

        //update em
        leds_update();
}

uint8_t leds_get(void)
{
        return led_register;
}

void leds_update(void)
{
        uint8_t mask_c = 0x1F; // 0001 1111
        uint8_t mask_b = 0x38; //0011 1000

        //delete old values
        PORTC &= ~mask_c;
        PORTB &= ~mask_b;
        //set new values
        PORTC |= (led_register >> 3) & mask_c;
        PORTB |= ((led_register << 3) & mask_b) | (PINB & ~mask_b);
}
