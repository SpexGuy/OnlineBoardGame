#pragma once

class Lock {
public:
	Lock() {}
	virtual bool lock() = 0;
	virtual long tryLock() = 0;
	virtual long tryLockFor(const unsigned int millis) = 0;
	virtual void unlock() = 0;
	virtual ~Lock() {}
};
