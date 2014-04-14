// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <string>
#include <CppUnitTest.h>
#include <btBulletDynamicsCommon.h>
#include <Address.h>

std::wstring ToString(const btVector3 &vec);

std::wstring ToString(const Address &addr);
