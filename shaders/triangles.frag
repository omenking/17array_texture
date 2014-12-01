#version 400 core
in vec2 uv;                      // Interpolated values from the vertex shaders
out vec3 color;                  // Ouput data
uniform sampler2DArray material; // Values that stay constant for the whole mesh.

void main(){
	color = texture(material,vec3(uv,0)).rgb;
}

