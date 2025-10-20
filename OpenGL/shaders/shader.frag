#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 trianglePos;

uniform float colorOffset;

void main()
{
	//FragColor = vec4(ourColor.x + colorOffset, ourColor.y + colorOffset, ourColor.z + colorOffset, 1.0);
	FragColor = vec4(trianglePos, 1.0);
};