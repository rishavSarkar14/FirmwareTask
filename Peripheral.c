#define CLK_SYS 16000000UL // Defining the CPU Frequency

#include <avr/io.h>      // Contains all the I/O Register Macros
#include <util/delay.h>  // Generates a Blocking Delay
#include <avr/interrupt.h> // Contains all interrupt vectors
#include <avr/eeprom.h>
#include <string.h>

#define BAUD 2400 // Desired Baud Rate
#define BAUD_PRESCALER (((CLK_SYS / (BAUD * 16UL))) - 1)

#define ASYNC (0<<UMSEL00) // USART Mode Selection
#define PARITY  (0<<UPM00) // USART Parity Bit Selection
#define STOP_BIT (0<<USBS0)      // USART Stop Bit Selection
#define DATA_BIT  (3<<UCSZ00) // USART Data Bit Selection

//#define RX_CMP_INT         (1<<RXCIE0)
//#define DAT_EMPTY_INT (1<<UDRIE0)

static int pointer = 0;

void USART_Init()
{
	// Set Baud Rate
	UBRR0H = BAUD_PRESCALER >> 8;
	UBRR0L = BAUD_PRESCALER;
	
	// Set Frame Format
	UCSR0C = ASYNC | PARITY | STOP_BIT | DATA_BIT;
	
	// Enable Receiver and Transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	//Enable Global Interrupts
	//sei();
}

void uart_putc(uint8_t src)
{
	while (( UCSR0A & (1<<UDRE0)) == 0) {}; // Do nothing until UDR is ready
	UDR0 = src;
}

uint8_t uart_getc()
{
  uint8_t data;
	while (( UCSR0A & (1<<RXC0)) == 0) {}; // Do nothing until data have been received
	data = UDR0;
  return data;
}

int main()
{
	USART_Init();
	while (1)
	{
    uint8_t ch = uart_getc();
    eeprom_write_byte((uint8_t*)pointer, ch);
    pointer++;
    eeprom_busy_wait();

    if(!ch)
    {
      for(int i = 0; i < pointer; i++)
      {
        uint8_t val = eeprom_read_byte((uint8_t*)i);
        uart_putc(val);
        eeprom_busy_wait();
      }
      pointer = 0;
    }
	}
	return 0;
}