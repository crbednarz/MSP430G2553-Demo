#include "World.h"


static uint8_t RenderTargetBuffer[DISPLAY_COLUMN_PAGES * 4];

void InitializeWorld(World* world)
{
	InitializeLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);

	world->Player.X = 64;
	world->Player.Y = 127;
}


static void TriggerSplash(World* world, uint8_t x, int8_t velocity)
{
	x >>= 1;
	velocity <<= 3;
	world->Liquids[x].Y += velocity;
	world->Liquids[x + 1].Y += velocity;
	world->Liquids[x - 1].Y += velocity;
	world->Liquids[x + 2].Y += velocity >> 1;
	world->Liquids[x - 2].Y += velocity >> 1;
	world->Liquids[x + 3].Y += velocity >> 2;
	world->Liquids[x - 3].Y += velocity >> 2;
}


static void StepPlayer(World* world)
{
	Entity* player = &world->Player;
	StepEntity(player);

	int centerX = player->X + 2;

	int liquidHeight = GetLiquidHeightAt(world->Liquids, centerX);

	if (player->Y >> 3 <= liquidHeight >> 3)
	{
		if (!player->IsTouchingGround && player->VelocityY < -8)
			TriggerSplash(world, centerX, -player->VelocityY);

		player->IsTouchingGround = true;
		player->VelocityY = 0;
		player->Y = liquidHeight;
	}
	else
	{
		player->IsTouchingGround = false;
	}
}

void StepWorld(World* world)
{
	StepLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH);

    StepPlayer(world);
    world->Time++;
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
	if (world->Player.IsTouchingGround)
	{
		world->Player.VelocityY = 10;
	}
}
