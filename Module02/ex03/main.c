#include <avr/io.h>
#include <avr/interrupt.h>

// UBRRn USART Baud Rate Register
// UCSR USART Control and Status Register
// UCSZ USART Character SiZe

#define UART_BAUDRATE 115200
#define F_CPU 16000000UL
#define MYUBRR ((unsigned char)((F_CPU / (16.0 * UART_BAUDRATE)) - 0.5)) // Table 19-12 (p.165)

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0))); // wait for empty buffer (p.159)
	UDR0 = c; // send char
}

char toUpCase(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 32;
	else if (c >= 'A' && c <= 'Z')
		return c + 32;
	return c;
}

void	uart_init(uint16_t ubrr)
{	
	UBRR0L = (uint8_t)(ubrr & 0xFF); // (p.162)
	UBRR0H = (uint8_t)(ubrr >> 8); // configure Baud rate (p.162)
	
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // activate TX (transmission) and RX (reception) and interrupt on reception
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Mode 8N1 (8bits, no parity, 1 stop bit) (p.162)
	sei();
}

ISR(USART_RX_vect)
{
	char received = UDR0; // read received char
	char changed = toUpCase(received); // transform received
	uart_tx(changed); // send changed char
}

int	main(void)
{
	uart_init(MYUBRR);

	while (1);
	return 0;
}
