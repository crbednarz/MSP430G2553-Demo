#ifndef RENDER_H_
#define RENDER_H_

#include "Display.h"
#include <stddef.h>
#include <stdint.h>
#include <msp430.h>

typedef struct
{
	uint8_t* Buffer;
	uint8_t StartColumn;
	uint8_t ColumnCount;
} RenderTarget;

inline void WriteValue(unsigned char value)
{
	UCB0TXBUF = value;
	__bis_SR_register(LPM3_bits | GIE);
}

inline void WriteData(unsigned char value)
{
	WriteValue(0xFF);
	WriteValue(value);
}

inline void WriteCommand(const unsigned char data)
{
	WriteValue(0x80);
	WriteValue(data);
}

void WriteValues(unsigned char* data, unsigned int length);

void BeginWrite();

void EndWrite();

void WriteCommands(const unsigned char* data, unsigned int length);

void ResetDisplayRAM_Vertical();

void ResetDisplayRAM_Page();

void DisplayRenderTarget(RenderTarget renderTarget);


#endif /* RENDER_H_ */
