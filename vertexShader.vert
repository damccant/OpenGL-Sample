#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}