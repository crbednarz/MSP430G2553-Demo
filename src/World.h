#ifndef WORLD_H_
#define WORLD_H_


#include "Display.h"
#include "Entity.h"
#include "Liquid.h"
#include "Tiles.h"

#define WORLD_LIQUIDS_LENGTH (DISPLAY_WIDTH / 2 + 2)


typedef struct
{
    LiquidPoint Liquids[WORLD_LIQUIDS_LENGTH];
    TileMap Map;
    Entity Player;
    uint32_t Time;

} World;


void InitializeWorld(World* world);

void StepWorld(World* world);

void RenderWorld(const World* world);

void ReportButtonPress(World* world);


#endif /* WORLD_H_ */
