#include "World.h"

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
    RenderLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);
}
