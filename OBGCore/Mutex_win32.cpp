#ifdef _WIN32
#include "Mutex.h"

Mutex::Mutex(const char *mutexName) :
	Lock()
{
	m_isAbandoned = false;
	m_lpsaAttributes = NULL;
	m_mutex = CreateMutex(m_lpsaAttributes, FALSE, mutexName);
}

bool Mutex::lock() {
	bool returnVal = true;
	unsigned long res = WaitForSingleObject(m_mutex, INFINITE);
	if (res == WAIT_ABANDONED) {
		m_isAbandoned = true;
		returnVal = false; 
	}
	return returnVal;
}

long Mutex::tryLock() {
	long ret = 0;
	unsigned long mutexStatus = WaitForSingleObject(m_mutex, 0);
	if(mutexStatus == WAIT_OBJECT_0)
		ret = 1;
	else if(mutexStatus == WAIT_ABANDONED)
		m_isAbandoned = true;
	return ret;
}

long Mutex::tryLockFor(const unsigned int millis) {
	long ret = 0;
	unsigned long mutexStatus = WaitForSingleObject(m_mutex, millis);
	if(mutexStatus == WAIT_OBJECT_0)
		ret = 1;
	else if(mutexStatus == WAIT_ABANDONED)
		m_isAbandoned = true;
	return ret;
}

void Mutex::unlock() {
	ReleaseMutex(m_mutex);
}

Mutex::~Mutex() {
	CloseHandle(m_mutex);
}

#endif
