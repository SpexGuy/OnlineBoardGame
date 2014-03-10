#pragma once

class PhysicsUpdate;

/** An interface for handling physics updates
 *
 */
class PhysicsUpdateListener {
public:

	virtual void handlePhysicsUpdate(PhysicsUpdate *update) = 0;

	virtual ~PhysicsUpdateListener() {}
};
