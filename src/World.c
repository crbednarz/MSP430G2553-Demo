#include "World.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


static uint8_t RenderTargetBuffer[DISPLAY_COLUMN_PAGES * 4];


static uint8_t GetBaseLiquidLevel(uint32_t time)
{
	return 100 + abs(((time >> 1) % 80) - 40);
}


void InitializeWorld(World* world)
{
	InitializeLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH, GetBaseLiquidLevel(world->Time));

	world->Player.X = 64;
	world->Player.Y = 127;

	world->Map.Tiles[5] = 0x5555U;
}


static void TriggerSplash(World* world, uint8_t x, int8_t velocity)
{
	x >>= 1;
	velocity <<= 4;

	velocity -= 5;

	world->Liquids[x].Y += velocity;
	world->Liquids[x + 1].Y += velocity;
	world->Liquids[x - 1].Y += velocity;
	world->Liquids[x + 2].Y += velocity >> 1;
	world->Liquids[x - 2].Y += velocity >> 1;
	world->Liquids[x + 3].Y += velocity >> 1;
	world->Liquids[x - 3].Y += velocity >> 1;
}


static void GenerateNewTileColumn(World* world)
{
	ShiftTilesLeft(&world->Map);

	if ((world->Time >> 2) % 10 < 2)
		world->Map.Tiles[TILE_MAP_WIDTH - 1] = 1 << ((rand() % 12) + 4);
}

static int GetTileMapOffsetX(const World* world)
{
	return world->Time % 4;
}

static bool IsPlayerTouchingTile(const World* world)
{
	const Entity* player = &world->Player;
	const TileMap* tileMap = &world->Map;

	int mapOffsetX = GetTileMapOffsetX(world);

	int startX = (player->X + 2 + mapOffsetX) >> 2;
	int endX = (player->X + 6 + mapOffsetX) >> 2;

	int startY = ((player->Y >> 2) + 2) >> 2;
	int endY = ((player->Y >> 2) + 6) >> 2;

	int x, y;
	for (x = startX; x <= endX; x++)
	{
		for (y = startY; y <= endY; y++)
		{
			if (((tileMap->Tiles[x] >> y) & 1) == 1)
				return true;
		}
	}
	return false;

}


static void StepPlayer(World* world)
{
	Entity* player = &world->Player;
	StepEntity(player);

	int centerX = player->X + 4;

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
    world->Time++;
	// Because liquids are two columns wide, we only shift the liquid array every other frame.
	if ((world->Time % 2) == 0)
	{
		ShiftLiquidsLeft(world->Liquids, WORLD_LIQUIDS_LENGTH);
		world->Liquids[WORLD_LIQUIDS_LENGTH - 1].Y = GetBaseLiquidLevel(world->Time + 4);
	}

	if ((world->Time % 4) == 0)
		GenerateNewTileColumn(world);

	StepLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH, GetBaseLiquidLevel(world->Time));

    StepPlayer(world);

    if (IsPlayerTouchingTile(world))
    {
    	memset(world, 0, sizeof(World));
    	InitializeWorld(world);
    }
}


void RenderWorld(const World* world)
{
	RenderTarget renderTarget;
	renderTarget.Buffer = RenderTargetBuffer;
	renderTarget.ColumnCount = 4;
	renderTarget.StartColumn = 0;

	for (renderTarget.StartColumn = 0; renderTarget.StartColumn < DISPLAY_WIDTH; renderTarget.StartColumn += renderTarget.ColumnCount)
	{
		// During the frames where we aren't shifting the liquid array over by one, we can offset the draw to make the movement appear smooth.
		RenderLiquids(world->Liquids, WORLD_LIQUIDS_LENGTH, renderTarget, world->Time % 2);

		RenderEntity(&world->Player, renderTarget);

		RenderTiles(&world->Map, renderTarget, GetTileMapOffsetX(world));

		DisplayRenderTarget(renderTarget);
	}
}


void ReportButtonPress(World* world)
{
	if (world->Player.IsTouchingGround)
	{
		world->Player.VelocityY = 12;
	}
}
