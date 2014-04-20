#define _USE_MATH_DEFINES
#include <math.h>
#include "ShakeStrategy.h"
#include "Entity.h"

void RandomizeRotationShakeStrategy::shake(Entity *ent) {
	btTransform trans = ent->getPhysicsBody()->getWorldTransform();
	float randY, randP, randR;
	randY = rand()/(float)RAND_MAX;
	randP = rand()/(float)RAND_MAX;
	randR = rand()/(float)RAND_MAX;

	randY *= 2 * M_PI;
	randP *= 2 * M_PI;
	randR *= 2 * M_PI;

	trans.setRotation(btQuaternion(randY, randP, randR));
	ent->getPhysicsBody()->setWorldTransform(trans);
}

ShakeStrategy *ShakeStrategy::defaultShakeStrategy = new DefaultShakeStrategy();
