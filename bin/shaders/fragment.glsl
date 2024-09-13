#version 330 core

out vec4 FragColor;

//in vec3 f_colour;
in vec2 TexCoord;
in vec2 map;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mapx;
uniform float mapy;
void main(){
	FragColor = mix(texture(texture1, vec2(TexCoord.x * mapx, TexCoord.y * mapy)), texture(texture2, vec2(1.0f - TexCoord.x,TexCoord.y)), 0.0);
}
