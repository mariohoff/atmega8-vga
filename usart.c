#include "usart.h"
#include <avr/io.h>


void usart_init(unsigned int ubrr)
{
        DDRD &= ~(1 << PD0);
        DDRD |= (1 << PD1);

        /* set baud rate */
        UBRRH = (unsigned char) (ubrr >> 8);
        UBRRL = (unsigned char) ubrr;

        /* enable receiver and transmitter */
        UCSRB = (1 << RXEN) | (1 << TXEN);
        /* set frame format: 8data, 2stop bit; */
        UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ0);
}

void usart_transmit(unsigned char data)
{
        /* wait for empty transmit buffer */
        while(!(UCSRA & (1 << UDRE)));

        /* Put data into buffer */
        UDR = data;
}

unsigned char usart_receive()
{
        /* wait for data to be received */
        while(!(UCSRA & (1 << RXC)));

        /* get and return received data */
        return UDR;
}
