#include <glm/gtc/matrix_transform.hpp>
#include <GraphicsContext.h>
#include <ILContainer.h>
#include <ILTexture.h>
#include <TextureMaterial.h>
#include <TextureShader.h>
#include <Vertex.h>
#include "GameManager.h"
#include "GraphicsManager.h"
#include "MousePointer.h"

using namespace std;
using namespace glm;

MousePointer::MousePointer() :
	height(0, 100, MOUSE_HEIGHT)
{
	/*vector<VertexPNT> points;
	vector<GLuint> faces;

	points.push_back(VertexPNT(vec3(-1,-1,0), vec3(0,0,1), vec2(0,0)));
	points.push_back(VertexPNT(vec3(1,-1,0), vec3(0,0,1), vec2(1,0)));
	points.push_back(VertexPNT(vec3(1,1,0), vec3(0,0,1), vec2(1,1)));
	points.push_back(VertexPNT(vec3(-1,1,0), vec3(0,0,1), vec2(0,1)));

	faces.push_back(0);
	faces.push_back(1);
	faces.push_back(2);
	faces.push_back(2);
	faces.push_back(3);
	faces.push_back(0);

	mesh = new GraphicsMesh(points, faces);

	ILContainer *image = new ILContainer("mouse.jpg");
	image->initialize();
	Texture *texture = new ILTexture(image, 0);
	
	TextureShader *texShader = new TextureShader();
	texShader->compileShader("NormalShader.vert");
	texShader->compileShader("NormalShader.frag");
	texShader->link();

	material = new TextureMaterial(texture, texShader);
*/
	screenPos = ivec2(0,0);
}

void MousePointer::update(int time) {
	height.update(time);
}

void MousePointer::render() {
	/*mat4 world = translate(mat4(1.0f), getWorldPos());
	material->bind();
	mesh->draw(world);*/
}

vec3 MousePointer::getWorldPos() {
	ivec2 size = GraphicsContext::inst()->getWindowSize();
	float x = float(screenPos.x)/size.x;
	float z = float(screenPos.y)/size.y;
	x = BOARD_SIZE * (2*x - 1);
	z = BOARD_SIZE * (2*z - 1);
	return vec3(x, height.get(), z);
}

void MousePointer::setWorldPos(const vec3 &worldPos) {
	height.set(worldPos.y);
	int x = (int)(((worldPos.x/BOARD_SIZE)+1)/2);
	int y = (int)(((worldPos.z/BOARD_SIZE)+1)/2);
	screenPos = ivec2(x, y);
}

MousePointer::~MousePointer() {

}
