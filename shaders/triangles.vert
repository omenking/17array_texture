#version 400 core
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 vertex_uv;

out vec2 uv;
uniform mat4 MVP;

void main(){
	gl_Position =  MVP * vec4(vertex_pos,1);
  uv          = vertex_uv;
}
