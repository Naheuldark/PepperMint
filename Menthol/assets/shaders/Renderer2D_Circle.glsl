// Circle Shader

#type vertex
#version 450 core

struct VertexOutput {
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout(location = 0) in vec3 iWorldPosition;
layout(location = 1) in vec3 iLocalPosition;
layout(location = 2) in vec4 iColor;
layout(location = 3) in float iThickness;
layout(location = 4) in float iFade;
layout(location = 5) in int iEntityId;

layout(std140, binding = 0) uniform Camera {
	mat4 uViewProjection;
};

layout (location = 0) out VertexOutput outVertex;
layout (location = 4) out flat int vEntityId;

void main() {
	outVertex.localPosition = iLocalPosition;
	outVertex.color = iColor;
	outVertex.thickness = iThickness;
	outVertex.fade = iFade;
	vEntityId = iEntityId;

	gl_Position = uViewProjection * vec4(iWorldPosition, 1.0);	
}

#type fragment
#version 450 core

struct VertexOutput {
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in VertexOutput inVertex;
layout (location = 4) in flat int vEntityId;

layout(location = 0) out vec4 oColor;
layout(location = 1) out int oEntityId;

void main() {
	// Calculate distance and fill it with white
	float distance = 1.0 - length(inVertex.localPosition);
	float circle = smoothstep(0.0, inVertex.fade, distance);
	circle *= smoothstep(inVertex.thickness + inVertex.fade, inVertex.thickness, distance);

	if (circle == 0) {
		discard;
	}

	// Set output color
	oColor = inVertex.color;
	oColor.a *= circle;

	oEntityId = vEntityId;
}
