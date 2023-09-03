#version 330 core

in vec3 vVertex;
in vec3 vertex_norm;


uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor;

out vec3 fColor;
out vec3 vertex_norm_fs;



void main() {
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
	vertex_norm_fs = vertex_norm;
	fColor = vColor; //Interpolate color
}
