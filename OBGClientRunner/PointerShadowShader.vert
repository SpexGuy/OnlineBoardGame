
#version 400

uniform mat4 m;
uniform mat4 mv;
uniform mat4 mvp;
uniform mat3 norm;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;

void main() {
	gl_Position = mvp * vec4(vPosition, 1.0);
	fPosition = vec3(m * vec4(vPosition, 1.0));
	fNormal = norm * vNormal;
	fTexCoord = vTexCoord;
}