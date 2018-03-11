#include "Entity.h"
#include "Utilities.h"

static const uint8_t BoatSprite[8] = { 0x06, 0x17, 0x37, 0x77, 0xFF, 0x07, 0x07, 0x06 };

void StepEntity(Entity* entity)
{
	entity->Y += entity->VelocityY;
	entity->VelocityY--;
}

void RenderEntity(const Entity* entity, RenderTarget renderTarget)
{
	// TODO: This is a placeholder box for entities.

	int startColumn = entity->X;
	int endColumn = entity->X + 8;

	startColumn = MAX(startColumn, renderTarget.StartColumn);
	endColumn = MIN(endColumn, renderTarget.StartColumn + renderTarget.ColumnCount);

	int top = entity->Y >> 2;
	int startPage = top / DISPLAY_WRITE_SIZE;

	if (endColumn <= startColumn)
		return;



	int x;
	for (x = startColumn; x < endColumn; x++)
	{
		uint8_t draw = BoatSprite[x - entity->X];

		if (((startPage + 1) % DISPLAY_COLUMN_PAGES) < DISPLAY_COLUMN_PAGES)
			renderTarget.Buffer[startPage + 1] |= draw >> (8 - (top % DISPLAY_WRITE_SIZE));
		renderTarget.Buffer[startPage] |= draw << (top % DISPLAY_WRITE_SIZE);
		startPage += DISPLAY_COLUMN_PAGES;
	}
}
