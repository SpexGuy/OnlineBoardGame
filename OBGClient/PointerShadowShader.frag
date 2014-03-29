
#version 400

uniform sampler2D colorTex;
uniform vec3 pointerPos;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

layout (location = 0) out vec4 FragColor;

void main() {
	FragColor = texture(colorTex, fTexCoord);
	vec3 dist = pointerPos - fPosition;
	if (dist.y > 0) {
		float distxz = sqrt(dist.x*dist.x + dist.z*dist.z);
		if (distxz < 0.01)
			FragColor = vec4(1,0,0,1);
		else
			FragColor *= vec4(vec3(0.5 + 0.5*smoothstep(dist.y/20,dist.y/10,distxz)), 1);
	}
	//FragColor = vec4(abs(dist), 1.0);
	//FragColor = vec4(abs(fNormal), 1.0);
}
