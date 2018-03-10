#ifndef ENTITY_H_
#define ENTITY_H_


#include "Render.h"
#include <stdbool.h>


typedef struct
{
	uint8_t X;
	uint8_t Y;
	int8_t VelocityY;
	uint8_t Height;
	bool IsTouchingGround;

} Entity;


void StepEntity(Entity* entity);

void RenderEntity(const Entity* entity, RenderTarget renderTarget);

#endif /* ENTITY_H_ */
