
#version 400

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 fColor;

void main() {
	gl_Position = mvp * vec4(vPosition, 1.0);
	//gl_Position = mvp * vec4(vPosition, 1.0);
	fColor = vNormal;
}