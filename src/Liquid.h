#ifndef LIQUID_H_
#define LIQUID_H_


#include "Render.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct
{
	uint8_t Y;
	int8_t Velocity;
} LiquidPoint;


void InitializeLiquids(LiquidPoint* liquids, size_t length, uint8_t baseLiquidLevel);

void RenderLiquids(const LiquidPoint* liquids, size_t length, RenderTarget renderTarget, int offsetX);

void StepLiquids(LiquidPoint* liquids, size_t length, uint8_t baseLiquidLevel);

uint8_t GetLiquidHeightAt(LiquidPoint* liquids, uint8_t x);

void ShiftLiquidsLeft(LiquidPoint* liquids, size_t length);

#endif /* LIQUID_H_ */
