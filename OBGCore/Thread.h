#pragma once

class Thread {
private:
	Thread();
	Thread(const Thread &that);
	Thread &operator=(const Thread &that);

	long threadId;
	void *threadHandle;

	int (*task)(void *param);
	void *param;
public:
	Thread(int (*task)(void *param), void *param);
	virtual bool start();
	int _run();
	virtual void waitForTerminate();
	virtual ~Thread();
};
