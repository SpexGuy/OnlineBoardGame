/**
	This class and its descendants are based on the following article:
	http://www.codeproject.com/Articles/421976/How-to-create-a-Simple-Lock-Framework-for-Cplusplu
 */
#pragma once

/**
 * An interface for a synchronizing lock
 */
class Lock {
public:
	Lock() {}
	virtual bool lock() = 0;
	virtual long tryLock() = 0;
	virtual long tryLockFor(const unsigned int millis) = 0;
	virtual void unlock() = 0;
	virtual ~Lock() {}
};

/**	A FunctionLock is useful to ensure the release of a
 *	lock at the end of a function, no matter what.
 *	Simply start the function with
 *	void foo() {
 *		FunctionLock lock(myLock)
 *		...
 *	}
 *	The FunctionLock will lock() myLock on entry,
 *	and will automatically release the lock when
 *	the function ends (via return or exception or anything).
 *
 *	If, for efficiency reasons, you wish to release the
 *	lock before the end of the function, you can call unlock().
 *	
 *	Despite the name, FunctionLock is NOT THREADSAFE, so
 *	each instance should only be handled by one thread.
 */
class FunctionLock {
protected:
	Lock &lock;
	bool locked;
public:
	inline FunctionLock(Lock &lock) :
		lock(lock),
		locked(false) {
		lock.lock();
		locked = true;
	}
	inline void unlock() {
		locked = false;
		lock.unlock();
	}
	inline ~FunctionLock() {
		if (locked)
			lock.unlock();
	}
};
