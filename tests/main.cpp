//program to demonstrate use of QTR driver
#define F_CPU 8000000UL

#include <avr/io.h>
#include "qtr_driver.h"
#include <util/delay.h>

int main()
{
	//We'll output QTR values continuously to PORTB
	DDRB=0xFF;
	DDRC=0xFF;
	//iMust call the driver initialization
	init_QTR_driver();

	//Forever output QTR value on PORTB
	while(1){
		//PORTB=get_QTR_value();
		unsigned char val = get_QTR_value();
	unsigned char sensor_5 = (val & 0x20);
	unsigned char sensor_2 = (val & 0x04);
	if(sensor_5 == 0x20 && sensor_2 == 0x00) {
		PORTC = 0x0A;
	}
	else if(val == 0x00)
	{
        PORTC = 0x0A;
        _delay_ms (200);
        PORTC = 0x0E;
        _delay_ms (200);
	}
	else if(val == 0xFF)
	{
        PORTC = 0x0E;
        _delay_ms (400);
	}
	else if(sensor_2==0x04) {
		PORTC = 0x0B;
		_delay_ms (10);
	}
	else if(sensor_5==0x00) {
		PORTC = 0x0E;
		_delay_ms (10);
	}
	else {
		PORTC = 0x0F;
	}

	}
}
