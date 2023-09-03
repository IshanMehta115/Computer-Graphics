#version 330 core

in vec3 vVertex;
in vec3 vertex_norm;


uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor;
uniform vec3 light_color;
uniform vec3 light_pos;

out vec3 fColor;


void main() {
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);

	vec3 light_pos_new = vec3(vModel * vec4(light_pos , 1.0));


	vec3 normal  = normalize(inverse(transpose(mat3(vModel))) * vertex_norm);
	vec3 frag_pos = vec3(vModel * vec4(vVertex , 1.0));
	vec3 light_dir = normalize(vec3(light_pos_new - frag_pos));
	float kd = 0.8;
	vec3 diffuse_light = kd * light_color * max(0, dot(light_dir, normal));


	vec3 result = (diffuse_light) * vColor;

	fColor = result; //Interpolate color
}
