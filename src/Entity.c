#include "Entity.h"
#include "Utilities.h"


void StepEntity(Entity* entity)
{
	entity->Y = (entity->Y + 1) & 0xFF;
}

void RenderEntity(const Entity* entity, RenderTarget renderTarget)
{
	// TODO: This is a placeholder box for entities.

	int startColumn = entity->X;
	int endColumn = entity->X + 4;

	startColumn = MAX(startColumn, renderTarget.StartColumn);
	endColumn = MIN(endColumn, renderTarget.StartColumn + renderTarget.ColumnCount);

	int top = entity->Y >> 2;
	int startPage = top / DISPLAY_WRITE_SIZE;

	if (endColumn <= startColumn)
		return;


	uint8_t invertedDraw = (1 << (top % DISPLAY_WRITE_SIZE)) - 1;

	int x;
	for (x = startColumn; x < endColumn; x++)
	{
		if (((startPage + 1) % DISPLAY_COLUMN_PAGES) < DISPLAY_COLUMN_PAGES)
			renderTarget.Buffer[startPage + 1] |= invertedDraw;
		renderTarget.Buffer[startPage] |= ~invertedDraw;
		startPage += DISPLAY_COLUMN_PAGES;
	}
}
