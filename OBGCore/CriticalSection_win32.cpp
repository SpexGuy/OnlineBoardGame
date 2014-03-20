#ifdef _WIN32
#include <assert.h>
#include "CriticalSection.h"

CriticalSection::CriticalSection() :
	Lock()
{
	InitializeCriticalSection(&m_criticalSection);
	m_lockCounter = 0;
}


bool CriticalSection::lock() {
	EnterCriticalSection(&m_criticalSection);
	m_lockCounter++;
	return true;
}

long CriticalSection::tryLock() {
	long ret = TryEnterCriticalSection(&m_criticalSection);
	if(ret)
		m_lockCounter++;
	return ret;
}

long CriticalSection::tryLockFor(const unsigned int millis)
{
	long ret=0;
	if(ret=TryEnterCriticalSection(&m_criticalSection)) {
		m_lockCounter++;
		return ret;
	} else {
		unsigned int startTime, timeUsed;
		unsigned int waitTime = millis;
		startTime = GetTickCount();
		while(WaitForSingleObject(m_criticalSection.LockSemaphore, waitTime) == WAIT_OBJECT_0) {
			if (ret = TryEnterCriticalSection(&m_criticalSection)) {
				m_lockCounter++;
				return ret;
			}
			timeUsed = GetTickCount()-startTime;
			waitTime = waitTime-timeUsed;
			startTime = GetTickCount();
		}
		return 0;
	}
}

void CriticalSection::unlock() {
	assert(m_lockCounter > 0);
	LeaveCriticalSection(&m_criticalSection);
	m_lockCounter--;
}


CriticalSection::~CriticalSection() {
	assert(m_lockCounter == 0);
	DeleteCriticalSection(&m_criticalSection);
}

#endif
