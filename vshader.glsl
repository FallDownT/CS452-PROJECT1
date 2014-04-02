#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

out vec4 pass_color;

void main(){
	gl_Position=projectionMatrix*viewMatrix*modelMatrix*vec4(in_position,1.0); 
	pass_color=in_color;
}
