// Line Shader

#type vertex
#version 450 core

struct VertexOutput {
	vec4 color;
};

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec4 iColor;
layout(location = 2) in int iEntityId;

layout(std140, binding = 0) uniform Camera {
	mat4 uViewProjection;
};

layout (location = 0) out VertexOutput outVertex;
layout (location = 1) out flat int vEntityId;

void main() {
	outVertex.color = iColor;
	vEntityId = iEntityId;

	gl_Position = uViewProjection * vec4(iPosition, 1.0);	
}

#type fragment
#version 450 core

struct VertexOutput {
	vec4 color;
};

layout (location = 0) in VertexOutput inVertex;
layout (location = 1) in flat int vEntityId;

layout(location = 0) out vec4 oColor;
layout(location = 1) out int oEntityId;

void main() {
	oColor = inVertex.color;
	oEntityId = vEntityId;
}
