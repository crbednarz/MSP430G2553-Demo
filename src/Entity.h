#ifndef ENTITY_H_
#define ENTITY_H_


#include "Render.h"


typedef struct
{
	uint8_t X;
	uint8_t Y;
	uint8_t Height;

} Entity;


void StepEntity(Entity* entity);

void RenderEntity(const Entity* entity, RenderTarget renderTarget);

#endif /* ENTITY_H_ */
