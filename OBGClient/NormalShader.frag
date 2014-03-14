
#version 400

uniform sampler2D colorTex;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

layout (location = 0) out vec4 FragColor;

void main() {
	FragColor = texture(colorTex, fTexCoord);
	//FragColor = vec4(abs(fNormal), 1.0);
}
