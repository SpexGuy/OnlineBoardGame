#include <glm/glm.hpp>
#include <GraphicsMesh.h>
#include <Material.h>
#include <Renderable.h>

#define MOUSE_HEIGHT 5

class MousePointer :
	public Renderable
{
private:
	glm::ivec2 screenPos;
	float height;
	Material *material;
	GraphicsMesh *mesh;

public:
	MousePointer();
	virtual void render();
	virtual ~MousePointer();

	void setWorldPos(const glm::vec3 &worldPos);
	glm::vec3 getWorldPos();

	void setScreenPos(const glm::ivec2 &screenPos) { this->screenPos = screenPos; }
	glm::ivec2 getScreenPos() { return screenPos; }
};
