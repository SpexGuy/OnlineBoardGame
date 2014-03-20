#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <btBulletDynamicsCommon.h>

bool loadCollisionMesh(std::istream *source,
					   std::vector<btVector3> *outPoints,
					   std::vector<int> *outTrigs);

