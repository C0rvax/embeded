#include "i2c.h"
#include "seven.h"
#include <util/delay.h>

volatile uint8_t timerOk = 0;

void	init_timer(void)
{
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC prescaler 1024
	OCR1A = 15624; // 30Hz
	TIFR1 |= (1 << OCF1A); // Clear any pending interrupt
}

int main(void)
{
    i2c_init();   // Initialisation I2C
    init_segments();  // Configuration des segments en sortie
    init_digits(); // Configuration des digits en sortie

	uint16_t number = 0;
    while (1)
	{
		init_timer();
		while (!(TIFR1 & (1 << OCF1A)))
			display_number(number);
		TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Stop the timer
		number = (number + 1) % 10000;
	}
}
