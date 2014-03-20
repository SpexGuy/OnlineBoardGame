#ifdef _WIN32
#include "Semaphore.h"

Semaphore::Semaphore(unsigned int count, const char *semName) :
	Lock()
{
	m_lpsaAttributes = NULL;
	m_count = count;
	m_sem = CreateSemaphore(m_lpsaAttributes, count, count, semName);
}

bool Semaphore::lock() {
	WaitForSingleObject(m_sem, INFINITE);
	return true;
}

long Semaphore::tryLock() {
	long ret = 0;
	if(WaitForSingleObject(m_sem, 0) == WAIT_OBJECT_0 )
		ret = 1;
	return ret;
}

long Semaphore::tryLockFor(const unsigned int millis) {
   long ret = 0;
   if( WaitForSingleObject(m_sem, millis) == WAIT_OBJECT_0)
      ret = 1;
   return ret;
}

void Semaphore::unlock() {
   ReleaseSemaphore(m_sem, 1, NULL);
}

long Semaphore::release(long count, long *retPreviousCount) {
   return ReleaseSemaphore(m_sem, count, retPreviousCount); 
}

Semaphore::~Semaphore() {
	CloseHandle(m_sem);
}

#endif
