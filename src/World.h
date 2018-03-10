#ifndef WORLD_H_
#define WORLD_H_


#include "Display.h"
#include "Liquid.h"


#define WORLD_LIQUIDS_LENGTH (DISPLAY_WIDTH / 2 + 1)


typedef struct
{
    LiquidPoint Liquids[WORLD_LIQUIDS_LENGTH];

} World;


void InitializeWorld(World* world);

void StepWorld(World* world);

void RenderWorld(World* world);

void ReportButtonPress(World* world);


#endif /* WORLD_H_ */
