// Basic texture shader

#type vertex
#version 330 core
			
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec4 iColor;
layout(location = 2) in vec2 iTexCoord;
layout(location = 3) in float iTexIndex;
layout(location = 4) in float iTilingFactor;

uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out float vTilingFactor;

void main() {
	vColor = iColor;
	vTexCoord = iTexCoord;
	vTexIndex = iTexIndex;
	vTilingFactor = iTilingFactor;
	gl_Position = uViewProjection * vec4(iPosition, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in float vTilingFactor;

uniform sampler2D uTextures[32];

void main() {
	oColor = texture(uTextures[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;
}