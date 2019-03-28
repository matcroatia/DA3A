/*
 * Assignment 3.c
 *
 * Created: 3/27/2019 6:20:28 PM
 * Author : Mat Tomljenovic
 */ 





#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

                                         
void USART_init( unsigned int ubrr );	//function for unsigned integer
void USART_tx_string( char *data );		//function for char data		

volatile int Count;						//integer variable count
char outs[60];							//number of character to be allowed
int n;									//integer declaration
float PI;								//floating point number declaration
char str[] = "The Sleeper Has Awakened!"; //string to be displayed
char empty[] = " ";						//declaration to print blank space
/* MAIN FUNCTION */

int main(void)
{
	Count  = 0;								//count used for timer
	TIMSK0 |= (1 << TOIE0);					//interrupt used when timer overflows
	sei ();									//enable interrupts
	TCCR0A = 0x00;							//enable normal mode							
	TCCR0B = 0x05;							//set pre-scaler to 1024	
	USART_init(BAUD_PRESCALLER);			//initiate baud pre-scaler											
	USART_tx_string("Connected!\r\n");		//displays if serial is connected to output                          
	while(1);
}
/* INIT USART (RS-232)  */
void USART_init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);		//baud rate register for high byte	
	UBRR0L = (unsigned char)ubrr;			//baud rate register for low byte
	UCSR0B = (1 << TXEN0);					//Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);					//asynchronous 8 N 1
}
/* SEND A STRING TO THE RS-232 */
void USART_tx_string( char *data )
{
	while ((*data != '\0'))					//wait while data transmission is ready
	{
		while (!(UCSR0A & (1 <<UDRE0)));	
		UDR0 = *data;						//store data into register UDR0
		data++;
	}
}
ISR (TIMER0_OVF_vect)						
{
	while (Count < 61)						//while loop to count
	{
		if ((TIFR0 & 0x01) == 1)			//sets overflow flag to 1
		{
			TIFR0 = 0x01;					
			Count++;						//increments count 
		}
	}
	if (Count > 60)							//while loop to count above 60
	{
		
		n = rand();							//integer used to generate random number
		PI = 3.1415;						//declaration for floating point decimal 
		dtostrf(PI, 6, 6, outs);			//print PI and declare how many decimal points print
		USART_tx_string(outs);				//prints declared string
		USART_tx_string(empty);				//print a blank space
		snprintf(outs, sizeof(outs), "%3d\r\n" , n);	//converts random number to integer
		USART_tx_string(outs);				//prints out PI
		USART_tx_string(empty);				//prints out blank space
		USART_tx_string(str);				//prints out random number
		USART_tx_string(empty);				//prints out blank space
		
		
		Count = 0;							//resets counter back to 0
	}
}