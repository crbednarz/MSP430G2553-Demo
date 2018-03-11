#include "Tiles.h"
#include <stdbool.h>


void RenderTiles(const TileMap* tileMap, RenderTarget renderTarget, int offsetX)
{
	int startX = renderTarget.StartColumn;
	int endX = renderTarget.StartColumn + renderTarget.ColumnCount;
	int x;
	int dstIndex = 0;
	for (x = startX; x < endX; x++)
	{
		uint16_t tileColumn = tileMap->Tiles[(x + offsetX) >> 2];
		int nextColumnDstIndex = dstIndex + DISPLAY_COLUMN_PAGES;

		int spriteColumn = (x + offsetX) % TILE_SIZE;
		bool spriteEdge = spriteColumn == 0 || spriteColumn == 3;

		while (tileColumn != 0)
		{
			if (tileColumn & 1 == 1)
				renderTarget.Buffer[dstIndex] ^= spriteEdge ? 0x6 : 0xF;
			tileColumn >>= 1;

			if (tileColumn & 1 == 1)
				renderTarget.Buffer[dstIndex] ^= spriteEdge ? 0x60 : 0xF0;
			tileColumn >>= 1;
			dstIndex++;
		}

		dstIndex = nextColumnDstIndex;
	}

}


void ShiftTilesLeft(TileMap* tileMap)
{
	int x;
	for (x = 1; x < TILE_MAP_WIDTH; x++)
		tileMap->Tiles[x - 1] = tileMap->Tiles[x];

	tileMap->Tiles[TILE_MAP_WIDTH - 1] = 0;
}
