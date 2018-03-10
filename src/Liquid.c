#include "Liquid.h"
#include <math.h>
#include <stdbool.h>

void InitializeLiquids(LiquidPoint* liquids, size_t length)
{
	unsigned int i;
	for (i = 0; i < length; i++)
	{
		liquids[i].Y = BASE_LIQUID_LEVEL;
		liquids[i].Velocity = 0;
	}
}


void RenderLiquids(const LiquidPoint* liquids, size_t length, RenderTarget renderTarget)
{
	int masks[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255 };

	unsigned int x;
	unsigned int y;
	unsigned int dstIndex = 0;
	for (x = renderTarget.StartColumn; x < (renderTarget.StartColumn + renderTarget.ColumnCount); x++)
	{
		uint8_t fill = 0xFF;

		unsigned int srcX = x >> 1;
		unsigned int srcY = (liquids[srcX].Y + liquids[srcX + 1].Y) >> 3;
		int srcWriteY = srcY >> 3;
		for (y = 0; y < DISPLAY_COLUMN_PAGES; y++)
		{
			if (srcWriteY == y)
			{
				renderTarget.Buffer[dstIndex] = masks[srcY % DISPLAY_WRITE_SIZE];
				fill = 0x00;
			}
			else
			{
				renderTarget.Buffer[dstIndex] = fill;
			}

			dstIndex++;
		}
	}
}

void StepLiquids(LiquidPoint* liquids, size_t length)
{
	static const int DECAY_BIT_SHIFT = 3;

	unsigned int x = DISPLAY_WIDTH >> 1;
	int lastY = liquids[0].Y - liquids[0].Velocity;
	LiquidPoint current = liquids[0];
	for (x = 0; x < length; x++)
	{
		uint8_t nextY;
		if (x + 1 == length)
			nextY = current.Y - current.Velocity;
		else
			nextY = liquids[x + 1].Y;
		uint16_t neighbors = lastY + nextY;

		lastY = current.Y;

		int height = current.Y;
		int velocity = current.Velocity;

		velocity += neighbors - (height << 1);

		int newHeight = velocity + height - BASE_LIQUID_LEVEL;
		newHeight = (((newHeight << DECAY_BIT_SHIFT) - newHeight) >> DECAY_BIT_SHIFT) + BASE_LIQUID_LEVEL;
		velocity = newHeight - height;

		current.Y = newHeight;
		current.Velocity = velocity;

		liquids[x] = current;
		current = liquids[x + 1];
	}
}


int8_t GetLiquidHeightAt(LiquidPoint* liquids, uint8_t x)
{
	x >>= 1;
	return (liquids[x].Y + liquids[x + 1].Y) >> 1;
}

