// Basic texture shader

#type vertex
#version 330 core
			
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec4 iColor;
layout(location = 2) in vec2 iTexCoord;

uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
	vColor = iColor;
	vTexCoord = iTexCoord;
	gl_Position = uViewProjection * vec4(iPosition, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;

uniform vec4 uColor;
uniform float uTilingFactor;
uniform sampler2D uTexture;

void main() {
	// oColor = texture(uTexture, vTexCoord * uTilingFactor) * uColor;
	oColor = vColor;
}