#include <glm/glm.hpp>

class VertexPNT {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	//simple assigning ctor
	VertexPNT(const glm::vec3 &position,
			  const glm::vec3 &normal,
			  const glm::vec2 &texCoord)
		 :  position(position),
			normal(normal),
			texCoord(texCoord)
	{}
};
