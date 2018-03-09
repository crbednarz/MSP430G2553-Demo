#include "Liquid.h"
#include "Display.h"
#include "Render.h"
#include <math.h>
#include <stdbool.h>

void InitializeLiquidLevel(LiquidPoint* liquids, size_t length, unsigned char liquidLevel)
{
	unsigned int i;
	for (i = 0; i < length; i++)
	{
		liquids[i].Y = liquidLevel;
		liquids[i].Velocity = 0;
	}
}


void RenderLiquids(LiquidPoint* liquids, size_t length)
{
	int masks[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255 };
	BeginWrite();

	WriteCommand(DISPLAY_SET_LOWER_COLUMN | 0);
	WriteCommand(DISPLAY_SET_HIGHER_COLUMN | 0);

	unsigned int x;
	unsigned int y;
	bool subStep = false;
	for (x = 0; x < DISPLAY_WIDTH; x++)
	{
		unsigned char fill = 0xFF;

		unsigned int srcX = x >> 1;
		unsigned int srcY = (liquids[srcX].Y >> 2);

		if (subStep)
			srcY = (srcY + (liquids[srcX + 1].Y >> 2)) >> 1;
		subStep = ~subStep;
		int srcWriteY = srcY >> 3;
		for (y = 0; y < DISPLAY_HEIGHT / DISPLAY_WRITE_SIZE; y++)
		{
			if (srcWriteY == y)
			{
				WriteData(masks[srcY % DISPLAY_WRITE_SIZE]);
				fill = 0x00;
			}
			else
			{
				WriteData(fill);
			}
		}
	}


	EndWrite();
}

void StepLiquids(LiquidPoint* liquids, size_t length)
{
	static const int TARGET_HEIGHT = 120;

	unsigned int x = DISPLAY_WIDTH >> 1;
	unsigned char lastY = liquids[0].Y;
	LiquidPoint current = liquids[1];
	for (x = 1; x < length - 1; x++)
	{
		unsigned short neighbors = lastY + liquids[x + 1].Y;
		lastY = current.Y;

		int height = current.Y;
		int velocity = current.Velocity;

		velocity += neighbors - (height << 1);

		int newHeight = velocity + height;
		newHeight = (((newHeight - TARGET_HEIGHT) * 7) >> 3) + TARGET_HEIGHT;
		velocity = newHeight - height;

		current.Y = newHeight;
		current.Velocity = velocity;

		liquids[x] = current;
		current = liquids[x + 1];
	}
}
