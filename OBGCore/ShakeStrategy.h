#pragma once

class Entity;

class ShakeStrategy {
public:
	static ShakeStrategy *defaultShakeStrategy;
	virtual void shake(Entity *ent) = 0;
	virtual ~ShakeStrategy() {}
};

class DefaultShakeStrategy :
	public ShakeStrategy
{
public:
	virtual void shake(Entity *ent) {}
};

class RandomizeRotationShakeStrategy :
	public ShakeStrategy
{
public:
	virtual void shake(Entity *ent);
	virtual ~RandomizeRotationShakeStrategy() {}
};
