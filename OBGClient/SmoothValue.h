#pragma once
#include <glm/glm.hpp>

template<typename T>
class SmoothValue {
private:
	T startVal;
	T target;
	T buffered;
	int startTime;
	int dTime;
	int time;

public:
	SmoothValue(int time, int dTime, T startVal);
	void set(const T &target);
	void setTarget(const T &target);
	void resetTimer();
	T get();
	void update(int time);
};

template<typename T>
SmoothValue<T>::SmoothValue(int time, int dTime, T startVal) :
	time(time), target(startVal),
	startTime(time), startVal(startVal),
	dTime(dTime), buffered(startVal)
{}

template<typename T>
void SmoothValue<T>::set(const T &val) {
	startVal = target;
	setTarget(val);
	resetTimer();
}

template<typename T>
void SmoothValue<T>::setTarget(const T &val) {
	target = val;
}

template<typename T>
void SmoothValue<T>::resetTimer() {
	startTime = time;
	buffered = startVal;
}

template<typename T>
T SmoothValue<T>::get() {
	return buffered;
}

template<typename T>
void SmoothValue<T>::update(int time) {
	this->time = time;
	buffered = startVal + (target - startVal)*glm::smoothstep(0.0f, float(dTime), float(time - startTime));
}
