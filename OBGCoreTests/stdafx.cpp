// stdafx.cpp : source file that includes just the standard includes
// OBGCoreTests.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

using namespace std;

wstring ToString(const btVector3 &vec) {
	wstringstream wstream;
	wstream << vec.getX() << ", " << vec.getY() << ", " << vec.getZ();
	return wstream.str();
}

wstring ToString(const Address &addr) {
	wstringstream wstream;
	wstream << addr.GetA() << '.' << addr.GetB() << '.' << addr.GetC() << '.' << addr.GetD() << ':' << addr.GetTCPPort();
	return wstream.str();
}
