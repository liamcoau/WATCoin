#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <string.h>

#define WATCH PD7

#define STRB1 PA1
#define STRB2 PA3
#define DAT1 PA0
#define DAT2 PA2

#include <stdio.h>
#include "I2C_slave.h"
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);
static int uart_putchar(char c, FILE *stream)
{
if (c == '\n') uart_putchar('\r', stream);
while( ! (UCSRA & (1<<UDRE)));
UDR = c;
return 0;
}



uint32_t generate_crc32 (uint32_t crc, unsigned char *buffer, int length) { 
    int i, j; 
    for (i=0; i<length; i++) { 
        crc = crc ^ *(buffer++); 
        for (j=0; j<8; j++) { 
		    if (crc & 1) 
		        crc = (crc>>1) ^ 0xEDB88320 ; 
		    else 
		        crc = crc >>1 ; 
        } 
    } 
    return crc; 
}


int main(){

DDRB |= (1<<PB0);
UCSRC |= ((1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0)); //Serial magic sauce. Set # of data bits, and the URSEL register to get around odd quirk in this gen. of AVR archetecture.
//Set UCPOL, UMSEL
UBRRL=207; // 38400bps.
UBRRH=0;

//UCSRA |= (1<<U2X);
SREG |= (1<<7); //These two lines enable interrupts.
sei();

I2C_init(0x32);

//UCSRC |= ((1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0));

UCSRB |= ((1<<TXEN)|(1<<RXEN)); // Enable RX,TX, and transmit interrupt.

stdout = &mystdout;

UDR='B';

PORTA |= (1<<PA2)|(1<<PA3)|(1<<PA4)|(1<<PA5)|(1<<PA6)|(1<<PA7);

DDRD &= ~(1<<PD7);

uint8_t t1bytes[128];
uint8_t t2bytes[128];


uint8_t indext1=0;
uint8_t indext2=0;

int bitdex1=6;//We're going to read 7 bits. of T1 data, in reverse. That's because it's sent as
				  // 1010 001 when it's really 0100 0101
uint8_t bitdex2=4;//We're going to read 5 bits of data, in reverse.
				  // So 1001 1 is getting packed as 1 1001.

memset(t1bytes,0,128); // Clear everything out.
memset(t2bytes,0,128);

uint8_t firstone=0;

uint8_t lastpinc=0xFF;


while(1){
	UDR='Z';
while(!(PIND & (1<<PD7))){ // While CP is low.

if(PINA != lastpinc){
lastpinc=PINA;
	if(! (lastpinc & (1<<STRB1))) { //If strobe is low...
		if( ! (lastpinc & (1<<DAT1))){ // If it's a one...
			//The first one is special - it marks the start sentinal.
			t1bytes[indext1] |= (1<<bitdex1);
			bitdex1--;
			firstone=1;
			//UDR=1;
		}else if(firstone){
			t1bytes[indext1] &= ~(1<<bitdex1);
			bitdex1--;
			//UDR=0;
		}
		if(bitdex1<0){

			bitdex1=6;
			indext1++;
		}
	}
}
}
if(indext1 > 0){
	uint32_t crcval=generate_crc32(0x01234567,t1bytes,indext1);
	printf("%x", crcval);
	memcpy(txbuffer,&crcval,sizeof(crcval));
	indext1=0;
	bitdex1=6;
	memset(t1bytes,0,128);
	firstone=0;
}

}

}
