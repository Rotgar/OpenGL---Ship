#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;
uniform vec3 offset;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z + offset.z, 1.0);
	ParticleColor = color;
}