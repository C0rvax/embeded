#ifndef ADC_H
# define ADC_H

# include <avr/io.h>

void	adc_init(void);
uint8_t adc_read(uint8_t channel);

#endif /* __ADC_H__ */
