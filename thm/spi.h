#ifndef SPI_H_
#define SPI_H_

#include <inttypes.h>
#include <avr/io.h>

#define SPIPORT PORTB
#define SPIPIN  PINB
#define SPIDDR  DDRB

#define SPISCK   PORTB5
#define SPIMISO  PORTB4 
#define SPIMOSI  PORTB3  
#define SPISS    PORTB2 

#define SPI_SS_HIGH() (SPIPORT |= (1<<SPISS))
#define SPI_SS_LOW() (SPIPORT &= ~(1<<SPISS))

void spi_init(void);
uint8_t spi_readwrite(uint8_t data);
uint8_t spi_read(void);

#endif

