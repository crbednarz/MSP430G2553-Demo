#ifndef LIQUID_H_
#define LIQUID_H_

#include "stddef.h"
#include "stdint.h"
#include "Render.h"

#define BASE_LIQUID_LEVEL 120


typedef struct
{
	uint8_t Y;
	int8_t Velocity;
} LiquidPoint;


void InitializeLiquids(LiquidPoint* liquids, size_t length);

void RenderLiquids(const LiquidPoint* liquids, size_t length, RenderTarget renderTarget);

void StepLiquids(LiquidPoint* liquids, size_t length);

int8_t GetLiquidHeightAt(LiquidPoint* liquids, uint8_t x);



#endif /* LIQUID_H_ */
