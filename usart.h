#ifndef USART_H
#define USART_H

/* init with baud 9600 and 8n1 */
void usart_init(unsigned int ubrr);

void usart_transmit(unsigned char data);
unsigned char usart_receive(void);

#endif
