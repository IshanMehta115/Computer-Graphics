#version 330 core

in vec3 fColor;
in vec3 vertex_norm_fs;
out vec4 outColor;
void main(void) {

        vec3 normal_color;
        for(int i=0;i<3;i++){
                normal_color[i] = (vertex_norm_fs[i] + 1)/2;
        }

        normal_color = normalize(normal_color);

        vec3 result = (normal_color) * fColor;
        outColor = vec4(result, 1.0);
}
