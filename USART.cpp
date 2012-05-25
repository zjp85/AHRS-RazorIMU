#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"

namespace USART_FRAME{
	//Variable for recieving state machine
	State state = USART_FRAME::WAIT_STX;
	uint8_t frameLength;
	uint8_t receivedData[FRAME_MAX_LENGTH];
	uint8_t dataIndx = 0;
};

using namespace USART_FRAME;

USART::USART(int baud){
	//Calcule baudrate register value
	long br = ((F_CPU + baud * 8L) / (baud * 16L) - 1);

	//Disable double transmission speed
	UCSR0A &= ~_BV(U2X0);

	//Enable receiver, transmitter and rx interrup
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);

	//Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);

	//Set baud rate. IMPORTANT: The Baud Rate must be set after the transmitter is enabled
	UBRR0 = br;
}

void USART::send(uint8_t byte){
	//wait while previous byte is completed
	while(!(UCSR0A & (1<<UDRE0)));
	// Transmit data
	UDR0 = byte;
}

void USART::send(uint8_t* buffer, uint8_t size){
	while(size--)
		send(*(buffer++));
}

void USART::sendFrame(uint8_t* buffer, uint8_t size){
	if(size){
		send(STX);
		while(size--){
			if(*buffer == DLE || *buffer == STX) send(DLE);
			send(*buffer++);
		}
	}
}

uint8_t USART::receive(){
	// Wait for byte to be received
	while(!(UCSR0A & (1<<RXC0)));
	// Return received data
	return UDR0;
}

//State machine for receive data interrupt
ISR(USART_RX_vect){
	PORTB ^= _BV(PB5);

	switch(state){
		case WAIT_STX:
			if(UDR0 == STX)
				state = RECIEVING_LENGTH;	
		break;

		case RECIEVING_LENGTH:
			switch(UDR0){
				case STX:
					state = WAIT_STX;
				break;

				case DLE:
					state = DISCARD_DLE_L;
				break;

				default:
					frameLength = UDR0;
					state = RECIEVING_DATA;
			}
		break;

		case RECIEVING_DATA:
			switch(UDR0){
				case STX:
					state = WAIT_STX;
				break;

				case DLE:
					state = DISCARD_DLE_D;
				break;

				default:
					receivedData[dataIndx++] = UDR0;
					if(dataIndx >= frameLength){
						state = WAIT_STX;
						dataIndx = 0;
					}
			}
		break;

		case DISCARD_DLE_L:
			frameLength = UDR0;
			state = RECIEVING_DATA;
		break;

		case DISCARD_DLE_D:
			receivedData[dataIndx++] = UDR0;

			if(dataIndx >= frameLength){
				state = WAIT_STX;
				dataIndx = 0;
			}
			else
				state = RECIEVING_DATA;
		break;
	}
}
