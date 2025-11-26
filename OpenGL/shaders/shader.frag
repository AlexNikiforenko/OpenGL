#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform float colorOffset;
uniform float mixValue;
uniform float time;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
	FragColor = mix(texture(texture1 , texCoord), texture(texture2, vec2(1.0 - texCoord.x, texCoord.y)), mixValue) ;
	//FragColor = vec4(ourColor.x + colorOffset, ourColor.y + colorOffset, ourColor.z + colorOffset, 1.0);
	//FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
	//FragColor = vec4(trianglePos, 1.0);
};