#ifdef _WIN32
#include "Event.h"

Event::Event(bool manualReset, bool initialState, char *name) {
	eventHandle = CreateEvent(NULL, manualReset, initialState, name);
}

void Event::wait() {
	WaitForSingleObject(eventHandle, INFINITE);
}

void Event::wait(const unsigned long millis) {
	WaitForSingleObject(eventHandle, millis);
}

void Event::reset() {
	ResetEvent(eventHandle);
}

void Event::notify() {
	SetEvent(eventHandle);
}

Event::~Event() {
	CloseHandle(eventHandle);
}

#endif
