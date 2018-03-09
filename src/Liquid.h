#ifndef LIQUID_H_
#define LIQUID_H_

#include "stddef.h"
#include "stdint.h"

typedef struct
{
	unsigned char Y;
	signed char Velocity;
} LiquidPoint;


void InitializeLiquidLevel(LiquidPoint* liquids, size_t length, unsigned char liquidLevel);

void RenderLiquids(LiquidPoint* liquids, size_t length);

void StepLiquids(LiquidPoint* liquids, size_t length);




#endif /* LIQUID_H_ */
