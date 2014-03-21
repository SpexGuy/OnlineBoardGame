#pragma once
#ifdef _WIN32
#	include <Windows.h>
#endif
#include "Lock.h"

/**	A Mutex is a synchronization primitive which ensures
 *	that only one thread has the lock at a time.
 */
class Mutex :
	public Lock
{
public:
	Mutex(const char *mutexName = NULL);
	virtual bool lock();
	virtual long tryLock();
	virtual long tryLockFor(const unsigned int millis);
	virtual void unlock();
	inline bool isAbandoned() { return m_isAbandoned; }
	virtual ~Mutex();
private:
	Mutex &operator=(const Mutex &b);
	Mutex(const Mutex &b);

#ifdef _WIN32
	/// Mutex
	HANDLE m_mutex;
	/// Creation Security Info
	LPSECURITY_ATTRIBUTES m_lpsaAttributes;
#endif
	/// Flag for whether this mutex is abandoned or not.
	bool m_isAbandoned;
};
