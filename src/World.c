#include "World.h"

static uint8_t RenderTargetBuffer[DISPLAY_COLUMN_PAGES * 4];

void InitializeWorld(World* world)
{
	InitializeLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);

	world->Player.Y = 127;
}


static void StepPlayer(World* world)
{
	StepEntity(&world->Player);


}

void StepWorld(World* world)
{
    StepLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);
    StepPlayer(world);
}


void RenderWorld(const World* world)
{
	RenderTarget renderTarget;
	renderTarget.Buffer = RenderTargetBuffer;
	renderTarget.ColumnCount = 4;
	renderTarget.StartColumn = 0;

	for (renderTarget.StartColumn = 0; renderTarget.StartColumn < DISPLAY_WIDTH; renderTarget.StartColumn += renderTarget.ColumnCount)
	{
		RenderLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH, renderTarget);

		RenderEntity(&world->Player, renderTarget);

		DisplayRenderTarget(renderTarget);
	}
}


void ReportButtonPress(World* world)
{

}
