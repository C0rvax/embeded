#include "adc.h"

void adc_init8(void)
{
    // Select the reference voltage AVCC
    ADMUX = (1 << REFS0) | (1 << ADLAR); // AVCC as reference, ADLAR -> 8-bit (see section 23.9.1 p.217)
    
    // Enable the ADC, prescale by 128 for good accuracy (~125kHz at 16MHz)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // (see section 24.9.2 of the doc)
}

void adc_init16(void)
{
    // Select the reference voltage AVCC
    ADMUX = (1 << REFS0); // AVCC as reference, 10-bit (see section 23.9.1 p.217)
    
    // Enable the ADC, prescale by 128 for good accuracy (~125kHz at 16MHz)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // (see section 24.9.2 of the doc)
}

uint8_t adc_read8(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Select the correct channel (0xF0 -> first 4 bits, 0x0F -> last 4 bits)
    ADCSRA |= (1 << ADSC); // Start the conversion (see section 24.9.2 of the doc)
    while (ADCSRA & (1 << ADSC)); // Wait for the conversion to finish
    return ADCH; // Return the 8-bit value (no 2 bits in ADCL)
}

uint16_t adc_read16(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Select the correct channel (0xF0 -> first 4 bits, 0x0F -> last 4 bits)
    ADCSRA |= (1 << ADSC); // Start the conversion (see section 24.9.2 of the doc)
    while (ADCSRA & (1 << ADSC)); // Wait for the conversion to finish
    return ADC; // Return the 10-bit value (all ADC)
}
