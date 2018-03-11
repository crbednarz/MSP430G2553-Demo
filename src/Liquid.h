#ifndef LIQUID_H_
#define LIQUID_H_


#include "Render.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#define BASE_LIQUID_LEVEL 120


typedef struct
{
	uint8_t Y;
	int8_t Velocity;
} LiquidPoint;


void InitializeLiquids(LiquidPoint* liquids, size_t length);

void RenderLiquids(const LiquidPoint* liquids, size_t length, RenderTarget renderTarget, int offsetX);

void StepLiquids(LiquidPoint* liquids, size_t length);

int8_t GetLiquidHeightAt(LiquidPoint* liquids, uint8_t x);

void ShiftLiquidsLeft(LiquidPoint* liquids, size_t length);

#endif /* LIQUID_H_ */
