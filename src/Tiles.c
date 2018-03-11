#include "Tiles.h"


void RenderTiles(const TileMap* tileMap, RenderTarget renderTarget, int offsetX)
{
	int startX = renderTarget.StartColumn;
	int endX = renderTarget.StartColumn + renderTarget.ColumnCount;
	int x, y;
	int srcIndex = startX * (TILE_MAP_HEIGHT / 8);
	int dstIndex = 0;
	for (y = 0; y < TILE_MAP_HEIGHT; y += 2)
	{
		for (x = startX; x < endX; x++)
		{
			renderTarget.Buffer[y >> 1] |= tileMap->Tiles[srcIndex] ? 0xF : 0;
			renderTarget.Buffer[y >> 1] |= tileMap->Tiles[srcIndex + 1] ? 0xF0 : 0;
			dstIndex++;
			srcIndex += 2;
		}
	}
}
