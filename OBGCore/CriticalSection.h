#pragma once
#include "Lock.h"
#ifdef _WIN32
#	include <Windows.h>
#endif

class CriticalSection :
	public Lock
{
public:
	CriticalSection();
	virtual bool lock();
	virtual long tryLock();
	virtual long tryLockFor(const unsigned int millis);
	virtual void unlock();
	virtual ~CriticalSection();
private:
	CriticalSection &operator=(const CriticalSection &b);
	CriticalSection(const CriticalSection& b);

#ifdef _WIN32
	CRITICAL_SECTION m_criticalSection;
#endif
	int m_lockCounter;
};
