// Quad Texture Shader

#type vertex
#version 450 core

struct VertexOutput {
	vec4 color;
	vec2 texCoord;
	float tilingFactor;
};

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec4 iColor;
layout(location = 2) in vec2 iTexCoord;
layout(location = 3) in float iTexIndex;
layout(location = 4) in float iTilingFactor;
layout(location = 5) in int iEntityId;

layout(std140, binding = 0) uniform Camera {
	mat4 uViewProjection;
};

layout (location = 0) out VertexOutput outVertex;
layout (location = 3) out flat float vTexIndex;
layout (location = 4) out flat int vEntityId;

void main() {
	outVertex.color = iColor;
	outVertex.texCoord = iTexCoord;
	outVertex.tilingFactor = iTilingFactor;
	vTexIndex = iTexIndex;
	vEntityId = iEntityId;

	gl_Position = uViewProjection * vec4(iPosition, 1.0);	
}

#type fragment
#version 450 core

struct VertexOutput {
	vec4 color;
	vec2 texCoord;
	float tilingFactor;
};

layout (location = 0) in VertexOutput inVertex;
layout (location = 3) in flat float vTexIndex;
layout (location = 4) in flat int vEntityId;

layout (binding = 0) uniform sampler2D uTextures[32];

layout(location = 0) out vec4 oColor;
layout(location = 1) out int oEntityId;

void main() {
	vec4 texColor = inVertex.color;
	switch(int(vTexIndex)) {
		case 0: texColor *= texture(uTextures[0], inVertex.texCoord * inVertex.tilingFactor); break;
		case 1: texColor *= texture(uTextures[1], inVertex.texCoord * inVertex.tilingFactor); break;
		case 2: texColor *= texture(uTextures[2], inVertex.texCoord * inVertex.tilingFactor); break;
		case 3: texColor *= texture(uTextures[3], inVertex.texCoord * inVertex.tilingFactor); break;
		case 4: texColor *= texture(uTextures[4], inVertex.texCoord * inVertex.tilingFactor); break;
		case 5: texColor *= texture(uTextures[5], inVertex.texCoord * inVertex.tilingFactor); break;
		case 6: texColor *= texture(uTextures[6], inVertex.texCoord * inVertex.tilingFactor); break;
		case 7: texColor *= texture(uTextures[7], inVertex.texCoord * inVertex.tilingFactor); break;
		case 8: texColor *= texture(uTextures[8], inVertex.texCoord * inVertex.tilingFactor); break;
		case 9: texColor *= texture(uTextures[9], inVertex.texCoord * inVertex.tilingFactor); break;
		case 10: texColor *= texture(uTextures[10], inVertex.texCoord * inVertex.tilingFactor); break;
		case 11: texColor *= texture(uTextures[11], inVertex.texCoord * inVertex.tilingFactor); break;
		case 12: texColor *= texture(uTextures[12], inVertex.texCoord * inVertex.tilingFactor); break;
		case 13: texColor *= texture(uTextures[13], inVertex.texCoord * inVertex.tilingFactor); break;
		case 14: texColor *= texture(uTextures[14], inVertex.texCoord * inVertex.tilingFactor); break;
		case 15: texColor *= texture(uTextures[15], inVertex.texCoord * inVertex.tilingFactor); break;
		case 16: texColor *= texture(uTextures[16], inVertex.texCoord * inVertex.tilingFactor); break;
		case 17: texColor *= texture(uTextures[17], inVertex.texCoord * inVertex.tilingFactor); break;
		case 18: texColor *= texture(uTextures[18], inVertex.texCoord * inVertex.tilingFactor); break;
		case 19: texColor *= texture(uTextures[19], inVertex.texCoord * inVertex.tilingFactor); break;
		case 20: texColor *= texture(uTextures[20], inVertex.texCoord * inVertex.tilingFactor); break;
		case 21: texColor *= texture(uTextures[21], inVertex.texCoord * inVertex.tilingFactor); break;
		case 22: texColor *= texture(uTextures[22], inVertex.texCoord * inVertex.tilingFactor); break;
		case 23: texColor *= texture(uTextures[23], inVertex.texCoord * inVertex.tilingFactor); break;
		case 24: texColor *= texture(uTextures[24], inVertex.texCoord * inVertex.tilingFactor); break;
		case 25: texColor *= texture(uTextures[25], inVertex.texCoord * inVertex.tilingFactor); break;
		case 26: texColor *= texture(uTextures[26], inVertex.texCoord * inVertex.tilingFactor); break;
		case 27: texColor *= texture(uTextures[27], inVertex.texCoord * inVertex.tilingFactor); break;
		case 28: texColor *= texture(uTextures[28], inVertex.texCoord * inVertex.tilingFactor); break;
		case 29: texColor *= texture(uTextures[29], inVertex.texCoord * inVertex.tilingFactor); break;
		case 30: texColor *= texture(uTextures[30], inVertex.texCoord * inVertex.tilingFactor); break;
		case 31: texColor *= texture(uTextures[31], inVertex.texCoord * inVertex.tilingFactor); break;
	}

	if (texColor.a == 0.0) {
		discard;
	}

	oColor = texColor;
	oEntityId = vEntityId;
}
