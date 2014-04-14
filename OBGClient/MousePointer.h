#include <glm/glm.hpp>
#include <GraphicsMesh.h>
#include <Material.h>
#include <Renderable.h>
#include "SmoothValue.h"

#define MOUSE_HEIGHT 5
#define MOUSE_LOW_HEIGHT 0.2

class MousePointer :
	public Renderable
{
private:
	glm::ivec2 screenPos;
	SmoothValue<float> height;
	Material *material;
	GraphicsMesh *mesh;

public:
	MousePointer();
	virtual void render();
	virtual void update(int time);
	virtual ~MousePointer();

	void setWorldPos(const glm::vec3 &worldPos);
	glm::vec3 getWorldPos();

	inline void setScreenPos(const glm::ivec2 &screenPos) { this->screenPos = screenPos; }
	inline const glm::ivec2 &getScreenPos() const { return screenPos; }

	inline void goLow() { height.set(float(MOUSE_LOW_HEIGHT)); }
	inline void goHigh() { height.set(float(MOUSE_HEIGHT)); }
};
