#include "prog.h"

void uart_init(void)
{
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8); // Configure the UBRR0H register (see section 20.11.1 of the doc)
    UBRR0L = (uint8_t)UBRR_VALUE; // Configure the UBRR0L register
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable reception and transmission (see section 20.11.2 of the doc)
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configure the frame format (8 bits) (see section 20.11.3 of the doc)
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))); // Wait until the register is ready (see section 20.11.2 of the doc)
    UDR0 = c; // Send the character
}

void uart_tx_string(const char *str)
{
    while (*str)
        uart_tx(*str++); // Send each character of the string
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0))); // Wait for a character to be received (see section 20.11.3 of the doc)
    return UDR0; // Return the received character
}


void	uart_print_hex16(uint16_t data)
{
	const char hex_digits[] = "0123456789abcdef";
    uart_tx(hex_digits[(data >> 12) & 0x0F]);
    uart_tx(hex_digits[(data >> 8) & 0x0F]);
	uart_tx(hex_digits[(data >> 4) & 0x0F]);
	uart_tx(hex_digits[data & 0x0F]);
}

void	uart_print_hex8(uint8_t data)
{
	const char hex_digits[] = "0123456789abcdef";
	uart_tx(hex_digits[(data >> 4) & 0x0F]);
	uart_tx(hex_digits[data & 0x0F]);
}

uint8_t hex_char_to_uint8(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 20;
}

uint16_t uart_rx_hex(void)
{
    uint16_t value = 0;
    char c;
    while (1)
	{
        c = uart_rx();
        if (c == '\n' || c == '\r') break;
        if (c == 127 || c == '\b')
		{
			value = (value >> 4);
			uart_tx_string("\b \b");
		}
		if (hex_char_to_uint8(c) != 20)
		{
			value = (value << 4) | hex_char_to_uint8(c);
			uart_tx(c);
		}
    }
    return value;
}
