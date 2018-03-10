#include "World.h"

static uint8_t RenderTargetBuffer[(DISPLAY_HEIGHT / DISPLAY_WRITE_SIZE) * 4];

void InitializeWorld(World* world)
{
	InitializeLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);
}


void StepWorld(World* world)
{
    StepLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);
}


void RenderWorld(World* world)
{
	RenderTarget renderTarget;
	renderTarget.Buffer = RenderTargetBuffer;
	renderTarget.ColumnCount = 4;
	renderTarget.StartColumn = 0;

	for (renderTarget.StartColumn = 0; renderTarget.StartColumn < DISPLAY_WIDTH; renderTarget.StartColumn += renderTarget.ColumnCount)
	{
		RenderLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH, renderTarget);

		DisplayRenderTarget(renderTarget);
	}
}
