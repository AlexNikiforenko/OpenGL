#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec3 trianglePos;

uniform float xOffset;
uniform float yOffset;

void main()
{
	gl_Position = vec4(aPos.x + xOffset, -aPos.y + yOffset, 0.0, 1.0);
	ourColor = aColor;
	trianglePos = aPos;
}