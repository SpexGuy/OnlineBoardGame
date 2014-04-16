#include <EntityManager.h>

class CollisionMeshRenderer;

class ClientEntityManager :
	public EntityManager
{
private:
	CollisionMeshRenderer *renderer;
public:

	virtual void start();

	virtual void update(int time);

};
