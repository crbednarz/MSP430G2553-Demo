#ifndef TILES_H_
#define TILES_H_


#include "Display.h"
#include "Render.h"
#include <stdint.h>


#define TILE_SIZE 4
#define TILE_MAP_PADDING 2
#define TILE_MAP_WIDTH (DISPLAY_WIDTH / TILE_SIZE + TILE_MAP_PADDING)


typedef struct
{
	uint16_t Tiles[TILE_MAP_WIDTH];
} TileMap;


void RenderTiles(const TileMap* tileMap, RenderTarget renderTarget, int offsetX);

void ShiftTilesLeft(TileMap* tileMap);

#endif /* TILES_H_ */
