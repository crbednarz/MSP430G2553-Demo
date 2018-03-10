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


void RenderLiquids(LiquidPoint* liquids, size_t length, RenderTarget renderTarget)
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
		for (y = 0; y < DISPLAY_HEIGHT / DISPLAY_WRITE_SIZE; y++)
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
	uint8_t lastY = liquids[0].Y;
	LiquidPoint current = liquids[1];
	for (x = 1; x < length - 1; x++)
	{
		uint16_t neighbors = lastY + liquids[x + 1].Y;
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
