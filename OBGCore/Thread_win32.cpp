#ifdef _WIN32
#include <Windows.h>
#include "Thread.h"

DWORD WINAPI Win32ThreadRunInternal(void *param) {
	return ((Thread *) param)->_run();
}

Thread::Thread(int (*task)(void *param), void *param) :
	param(param),
	task(task),
	threadHandle(NULL)
{
}

bool Thread::start() {
	DWORD id;
	threadHandle =
		CreateThread(NULL, 0, //default security attributes and stack size
			Win32ThreadRunInternal, this, //callback to this
			0, &id); //default flags and id output
	threadId = id;
	return threadHandle != NULL;
}

int Thread::_run() {
	return task(param);
}

void Thread::waitForTerminate() {
	WaitForSingleObject(threadHandle, INFINITE);
}

Thread::~Thread() {
}

#endif
