#version 330 core

in vec3 vVertex;
in vec3 vertex_norm;


uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;

out vec3 fColor;


void main() {
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);

	vec3 light_pos_new = vec3(vModel * vec4(light_pos , 1.0));
	vec3 cam_pos_new = cam_pos;


	float ka = 0.5;
	vec3 ambient_light = ka * light_color;



	vec3 normal  = normalize(inverse(transpose(mat3(vModel))) * vertex_norm);
	vec3 frag_pos = vec3(vModel * vec4(vVertex , 1.0));
	vec3 light_dir = normalize(vec3(light_pos_new - frag_pos));
	float kd = 0.5;
	vec3 diffuse_light = kd * light_color * max(0, dot(light_dir, normal));



	float ks = 0.9;
	vec3 view_dir = normalize(cam_pos_new  - frag_pos);
	vec3 h = normalize(light_dir + view_dir);
	vec3 specular_light = ks * light_color * pow(max(0, dot(h ,normal)), 32);

	vec3 result = (ambient_light + diffuse_light + specular_light) * vColor;

	fColor = result; //Interpolate color
}
