/*
 * spi.c
 *
 *  Created on: 24-Jan-2009
 *      Author: Neil MacMillan
 *
 *  Functions for using the AT90 as an SPI master.
 */

#include "spi.h"
#include "../Core/WProgram.h"
//#include "util/delay.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#if 1  // Arduino Uno definitions
#define MOSI 11
#define MISO 12
#define SCK 13
#define SS 10
#else  // Arduino Mega definitions
#define MOSI 51
#define MISO 50
#define SCK 52
#define SS 53
#endif

// wait for an SPI read/write operation to complete
#define SPI_WAIT()              while ((SPSR & _BV(SPIF)) == 0);

void SPI_Init()
{
	// The DDR operations are pretty fragile and doing this less awkwardly breaks SPI.  I don't care enough
	// to figure out why right now.  Note that SPI_SS is the actual SPI port's SS pin, which is not necessarily
	// the SS pin that's used for a given slave.  Anything that uses this driver must handle its own slave selection.
	// It must set its SS pin direction to output, set the pin low before doing an SPI operation, and set it high
	// when the SPI operation is complete.
	pinMode(MOSI, OUTPUT);
	pinMode(MISO, INPUT);
	pinMode(SCK, OUTPUT);
	pinMode(SS, OUTPUT);

	/* SPCR format:
	 *
	 * bit 7
	 * SPE - SPI enable
	 * SPIE - SPI interrupt enable
	 * DORD - Data order (0 MSB first; 1 LSB first)
	 * MSTR - Master (0 slave; 1 master)
	 * CPOL - Clock polarity (should be 0)
	 * CPHA - Clock phase (should be 0)
	 * SPR1 | SPR0 - Determines SCK frequency along with the SPI2X bit in SPSR.  See Table 17-4 in hardware manual [p178].
	 * bit 0
	 */

    // Set the AT90's SS pin high so that this board doesn't get set to slave mode.
	digitalWrite(SS, HIGH);

	SPCR = _BV(SPE) | _BV(MSTR);	// enable SPI, set as master, set prescaler to f(osc)/4

	digitalWrite(SS, LOW);
}

void SPI_ReadWrite_Block(uint8_t* data, uint8_t* buffer, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPDR = data[i];
          SPI_WAIT();
          buffer[i] = SPDR;
    }
}

void SPI_Write_Block(uint8_t* data, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPDR = data[i];
          SPI_WAIT();
    }
}

uint8_t SPI_Write_Byte(uint8_t byte)
{
    SPDR = byte;
    SPI_WAIT();
    return SPDR;
}
