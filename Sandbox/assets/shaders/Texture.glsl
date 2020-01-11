// Basic texture shader

#type vertex
#version 330 core
			
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec2 iTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec2 vTexCoord;

void main() {
	vTexCoord = iTexCoord;
	gl_Position = uViewProjection * uTransform * vec4(iPosition, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 oColor;

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main() {
	oColor = texture(uTexture, vTexCoord * 10.0) * vec4(1.0, 0.5, 0.3, 1.0);
}