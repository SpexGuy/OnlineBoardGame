#pragma once
#ifdef _WIN32
#	include <Windows.h>
#endif

class Event {
public:
	Event(bool manualReset, bool initialState, char *name = NULL);
	virtual void wait();
	virtual void wait(const unsigned long millis);
	virtual void reset();
	virtual void notify();
	virtual ~Event();

private:
	Event(const Event &a);
	Event &operator=(const Event &b);

#ifdef _WIN32
	HANDLE eventHandle;
#endif
};
