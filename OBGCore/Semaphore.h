#pragma once
#include "Lock.h"
#ifdef _WIN32
#	include <Windows.h>
#endif
 
/**	A Semaphore is a synchronization primitive which allows
 *	for a certain number of resources to be available for
 *	locking.
 */
class Semaphore :
	public Lock
{
public:
	Semaphore(unsigned int count = 1, const char *semName = "");
	virtual bool lock();
	virtual long tryLock();
	virtual long tryLockFor(const unsigned int millis);
	virtual void unlock();
	long release(long count, long * retPreviousCount);
	virtual ~Semaphore();
private:
	Semaphore(const Semaphore& b);
	Semaphore &operator=(const Semaphore &b);

#ifdef _WIN32
	/// Actual Semaphore
	HANDLE m_sem;
	/// Creation Info
	LPSECURITY_ATTRIBUTES m_lpsaAttributes;
#endif
	/// Semaphore Flag
	unsigned int m_count;
};
