#version 450

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;

layout (location = 0) out vec3 fragment_color;


void main() {
	gl_Position = vec4(vertex_position, 1.0f);
	fragment_color = vertex_color;
}