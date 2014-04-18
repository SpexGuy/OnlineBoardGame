
#version 400

uniform sampler2D colorTex;
uniform vec3 pointerPos;
uniform vec3 lightEyePos;

centroid in vec3 fWorldPosition;
centroid in vec3 fEyePosition;
centroid in vec3 fNormal;
centroid in vec2 fTexCoord;

layout (location = 0) out vec4 FragColor;

void ads() {
	vec3 color = FragColor.rgb;
	vec3 n = normalize(fNormal);
	vec3 returnColor = vec3(0.0f);
	if (!gl_FrontFacing)
		n = -n;
	vec3 s = normalize(lightEyePos - fEyePosition);
	float s_dot_n = max(dot(s, n), 0.0);

	vec3 specular = vec3(0.0);
	vec3 v = normalize(-fEyePosition);
	vec3 r = reflect(-s, n);
	specular = (s_dot_n > 0 ? vec3(pow(max(dot(r, v), 0.0), 50)) : vec3(0.0));
	returnColor +=
		0.5*color +
		0.5*color*s_dot_n +
		0.3*specular;

	FragColor = vec4(returnColor, FragColor.a);
}

void shadowAndLaser() {
	vec3 dist = pointerPos - fWorldPosition;
	if (dist.y > 0) {
		float distxz = sqrt(dist.x*dist.x + dist.z*dist.z);
		if (distxz < 0.1)
			FragColor = vec4(1,0,0,1);
		else if (pointerPos.y > 0)
			FragColor *= vec4(vec3(0.5 + 0.5*smoothstep(dist.y/(pointerPos.y),dist.y/(0.5*pointerPos.y),distxz)), 1);
	}
}

void main() {
	FragColor = texture(colorTex, fTexCoord);
	ads();
	shadowAndLaser();
	//FragColor = vec4(abs(dist), 1.0);
	//FragColor = vec4(abs(fNormal), 1.0);
}
