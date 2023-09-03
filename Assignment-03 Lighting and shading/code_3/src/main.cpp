//Assignment 03: Lighting and shading

/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/

#include "utils.h"
#include<vector>
#include<iostream>

using namespace std;

#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>


//Globals
int screen_width = 640, screen_height=640;
GLint vModel_uniform, vView_uniform, vProjection_uniform;
GLint vColor_uniform,light_pos_value,light_color_value,cam_pos_value;
glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations

double oldX, oldY, currentX, currentY;
bool isDragging=false;

void createCubeObject(unsigned int &, unsigned int &);

void setupModelTransformation(unsigned int &);
void setupViewTransformation(unsigned int &);
void setupProjectionTransformation(unsigned int &);
glm::vec3 getTrackBallVector(double x, double y);

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");

    //Get handle to color variable in shader
    vColor_uniform = glGetUniformLocation(shaderProgram, "vColor");
    if(vColor_uniform == -1){
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }

    light_pos_value = glGetUniformLocation(shaderProgram, "light_pos");
    if(light_pos_value == -1){
        fprintf(stderr, "Could not bind location: light_pos\n");
        exit(0);
    }

    light_color_value = glGetUniformLocation(shaderProgram, "light_color");
    if(light_color_value == -1){
        fprintf(stderr, "Could not bind location: light_color\n");
        exit(0);
    }

    cam_pos_value = glGetUniformLocation(shaderProgram, "cam_pos");
    if(cam_pos_value == -1){
        fprintf(stderr, "Could not bind location: cam_pos\n");
        exit(0);
    }

    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram);

    createCubeObject(shaderProgram, VAO);

    oldX = oldY = currentX = currentY = 0.0;
    int prevLeftButtonState = GLFW_RELEASE;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Get current mouse position
        int leftButtonState = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT);
        double x,y;
        glfwGetCursorPos(window,&x,&y);
        if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_RELEASE){
            isDragging = true;
            currentX = oldX = x;
            currentY = oldY = y;
        }
        else if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_PRESS){
            currentX = x;
            currentY = y;
        }
        else if(leftButtonState == GLFW_RELEASE && prevLeftButtonState == GLFW_PRESS){
            isDragging = false;
        }

        // Rotate based on mouse drag movement
        prevLeftButtonState = leftButtonState;
        if(isDragging && (currentX !=oldX || currentY != oldY))
        {
            glm::vec3 va = getTrackBallVector(oldX, oldY);
            glm::vec3 vb = getTrackBallVector(currentX, currentY);

            float angle = acos(std::min(1.0f, glm::dot(va,vb)));
            glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
            glm::mat3 camera2object = glm::inverse(glm::mat3(viewT*modelT));
            glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
            modelT = glm::rotate(modelT, angle, axis_in_object_coord);
            glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));

            oldX = currentX;
            oldY = currentY;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);

        {
            ImGui::Begin("Information");                          
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO); 
        
        glUniform3f(vColor_uniform, 0.5, 0.5, 0.5);
        glUniform3f(light_pos_value, 30.0, 0.0, 0.0);
        glUniform3f(light_color_value, 1.0, 0.0, 0.0);
        glUniform3f(cam_pos_value, 40.0, -40.0, 40.0);
        glDrawArrays(GL_TRIANGLES, 0, 5428*3);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}


float PI = 3.1415926;
int total_phi_steps = 61;
int total_theta_steps = 46;
float Radius = 20;
float X(float phi, float theta)
{
    return sin(phi) * cos(theta);
}
float Y(float phi, float theta)
{
    return sin(phi) * sin(theta);   
}
float Z(float phi, float theta)
{
    return cos(phi);
}
void sphere_parametric_equation(float phi, float theta, float *coordinates)
{
    coordinates[0] = Radius * X(phi, theta);
    coordinates[1] = Radius * Y(phi, theta);
    coordinates[2] = Radius * Z(phi, theta);
}
float get_phi_angle(int phi_step)
{
    return phi_step*(PI)/(total_phi_steps -1);
}
float get_theta_angle(int theta_step)
{
    return theta_step*(2*PI)/(total_theta_steps);
}

void createCubeObject(unsigned int &program, unsigned int &cube_VAO)
{
    glUseProgram(program);

    //Bind shader variables
    int vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }

    //Cube data
    // GLfloat cube_vertices[] = {10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10, //Front
    //                10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10}; //Back
    // GLushort cube_indices[] = {
    //             0, 1, 2, 0, 2, 3, //Front
    //             4, 7, 5, 5, 7, 6, //Back
    //             1, 6, 2, 1, 5, 6, //Left
    //             0, 3, 4, 4, 7, 3, //Right
    //             0, 4, 1, 4, 5, 1, //Top
    //             2, 6, 3, 3, 6, 7 //Bottom

    //             };













    //Generate VAO object
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    //Create VBOs for the VAO
    //Position information (data + format)
    int nVertices = 16284; //(6 faces) * (2 triangles each) * (3 vertices each)
    GLfloat *expanded_vertices = new GLfloat[nVertices*3];
    // for(int i=0; i<nVertices; i++) {
    //     expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
    //     expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
    //     expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
    // }

    int index = 0;


    float coordinates[3];

    for(int phi_step = 0;phi_step<total_phi_steps;phi_step++)
    {
        if(0==phi_step || (total_phi_steps-1)==phi_step) continue;

        if(1==phi_step)
        {
            for(int theta_step = 0;theta_step<total_theta_steps;theta_step++)
            {
                
                sphere_parametric_equation(get_phi_angle(phi_step - 1),get_theta_angle((0)%total_theta_steps),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step),get_theta_angle((theta_step + 1)%total_theta_steps),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step),get_theta_angle(theta_step),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];
            }   
        }
        else if((total_phi_steps - 2)==phi_step)
        {
            for(int theta_step = 0;theta_step<total_theta_steps;theta_step++)
            {
                sphere_parametric_equation(get_phi_angle(phi_step + 1),get_theta_angle(0),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step),get_theta_angle(theta_step),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step),get_theta_angle((theta_step + 1)%total_theta_steps),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];
            } 
        }

        if(1 < phi_step)
        {
            for(int theta_step = 0; theta_step<total_theta_steps;theta_step++)
            {
                sphere_parametric_equation(get_phi_angle(phi_step - 1),get_theta_angle(theta_step),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step),get_theta_angle(theta_step),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step),get_theta_angle((theta_step + 1)%total_theta_steps),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];




                sphere_parametric_equation(get_phi_angle(phi_step - 1),get_theta_angle(theta_step),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step),get_theta_angle((theta_step + 1)%total_theta_steps),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];

                sphere_parametric_equation(get_phi_angle(phi_step - 1),get_theta_angle((theta_step + 1)%total_theta_steps),coordinates);
                for(int i=0;i<3;i++) expanded_vertices[index++] = coordinates[i];
            }
        }
    }



    GLfloat *normals = new GLfloat[nVertices*3];

    for(int i=0;i<nVertices*3;i+=9){
        glm::vec3 p1 = {expanded_vertices[i], expanded_vertices[i + 1],expanded_vertices[i + 2]};
        glm::vec3 p2 = {expanded_vertices[i + 3], expanded_vertices[i + 4],expanded_vertices[i + 5]};
        glm::vec3 p3 = {expanded_vertices[i + 6], expanded_vertices[i + 7],expanded_vertices[i + 8]};

        glm::vec3 v1 = p1 - p3;
        glm::vec3 v2 = p2 - p3;

        v1 = glm::normalize(v1);
        v2 = glm::normalize(v2);
        glm::vec3 v3 = glm::cross(v1,v2);
        v3 = glm::normalize(v3);
        if(dot(v3, p1) < 0 || dot(v3, p2) < 0 || dot(v3, p3) < 0) v3*=-1;
        for(int j=0;j<9;j++){
            normals[j + i] = v3[j%3];
        }
    }






















    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);



    GLuint normal_VBO;
    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);





    delete []expanded_vertices;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void setupModelTransformation(unsigned int &program)
{
    //Modelling transformations (Model -> World coordinates)
    modelT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));//Model coordinates are the world coordinates

    //Pass on the modelling matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}


void setupViewTransformation(unsigned int &program)
{
    //Viewing transformations (World -> Camera coordinates
    //Camera at (0, 0, 100) looking down the negative Z-axis in a right handed coordinate system
    viewT = glm::lookAt(glm::vec3(40.0, -40.0, 40.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    //Pass-on the viewing matrix to the vertex shader
    glUseProgram(program);
    vView_uniform = glGetUniformLocation(program, "vView");
    if(vView_uniform == -1){
        fprintf(stderr, "Could not bind location: vView\n");
        exit(0);
    }
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}

void setupProjectionTransformation(unsigned int &program)
{
    //Projection transformation
    projectionT = glm::perspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 0.1f, 1000.0f);

    //Pass on the projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if(vProjection_uniform == -1){
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

glm::vec3 getTrackBallVector(double x, double y)
{
	glm::vec3 p = glm::vec3(2.0*x/screen_width - 1.0, 2.0*y/screen_height - 1.0, 0.0); //Normalize to [-1, +1]
	p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

	float mag2 = p.x*p.x + p.y*p.y;
	if(mag2 <= 1.0f)
		p.z = sqrtf(1.0f - mag2);
	else
		p = glm::normalize(p); //Nearest point, close to the sides of the trackball
	return p;
}

