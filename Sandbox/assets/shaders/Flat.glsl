// Basic flat color shader

#type vertex
#version 330 core

layout(location = 0) in vec3 iPosition;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

void main() {
	gl_Position = uViewProjection * uTransform * vec4(iPosition, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 oColor;

in vec4 vColor;

uniform vec3 uColor;

void main() {
	oColor = vec4(uColor, 1.0);
}