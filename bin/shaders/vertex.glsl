#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 colour;
layout (location = 1) in vec2 aTexCoord;

//out vec3 f_colour;
out vec2 TexCoord;

uniform mat4 transformMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
//	f_colour = colour;
	TexCoord = aTexCoord;
}
