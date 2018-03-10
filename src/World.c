#include "World.h"

void InitializeWorld(World* world)
{
    InitializeLiquidLevel(Liquids, LIQUIDS_LENGTH, 120);
}


void StepWorld(World* world)
{
    StepLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);
}

void RenderWorld(World* world)
{
    RenderLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);
}
