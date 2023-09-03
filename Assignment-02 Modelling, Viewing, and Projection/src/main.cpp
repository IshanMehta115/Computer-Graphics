#include "utils.h"
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
GLint vColor_uniform;
glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations
glm::vec4 camPosition;
glm::vec3 my_up_vector,my_right_vector;
char textKeyStatus[256];

float PI = 3.1415926;
float phi = PI/2 + PI/12, theta = PI/2 + PI/6;

bool is_perspective = true;

void createCubeObject(unsigned int &, unsigned int &);
void createAxesLine(unsigned int &, unsigned int &);

void setupModelTransformationCube(unsigned int &);
void setupModelTransformationAxis(unsigned int &program, float rot_angle, glm::vec3 rot_axis);
void setupViewTransformation(unsigned int &);
void setupProjectionTransformation(unsigned int &);


int my_index = 0;
vector<vector<float>> my_pre_set = {{0.0,100.0,0.0},
																	{0.0,0.0,92.0},
																	{92.0,0.0,0.0}};

bool my_is_pressed = true;



float my_get_radius(){
	return sqrt(camPosition.x * camPosition.x + camPosition.y * camPosition.y + camPosition.z * camPosition.z);
}
void my_scale(float my_scale_factor){
	camPosition[0]*=my_scale_factor;
	camPosition[1]*=my_scale_factor;
	camPosition[2]*=my_scale_factor;
}
glm::mat3 my_get_transpose(glm::mat3 mat){
	glm::mat3 res;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			res[i][j] = mat[j][i];
		}
	}
	return res;
}
// glm::mat3 my_mult(glm::mat3 m1, glm::mat3 m2){
// 	glm::mat3 res;

// 	for(int i=0;i<3;i++){
// 		for(int j=0;j<3;j++){
// 			res[i][j] = 0;
// 			for(int k=0;k<3;k++){
// 				res[i][j]+=m1[i][k]*m2[k][j];
// 			}
// 		}
// 	}
// 	return res;
// }
// float my_dot(glm::vec3 v1, glm::vec3 v2){
// 	float ans = 0;
// 	for(int i=0;i<3;i++) ans+=v1[i]*v2[i];
// 	return ans; 
// }
// void my_rotate(int option, float radians){
// 	radians/=10;

// 	float radius = my_get_radius();

// 	cout<<"radians = "<<radians<<"\n";
// 	cout<<"up\n";
// 	for(int i=0;i<3;i++){
// 		cout<<my_up_vector[i]<<", ";
// 	}
// 	cout<<"\n";
// 	cout<<"right\n";
// 	for(int i=0;i<3;i++){
// 		cout<<my_right_vector[i]<<", ";
// 	}
// 	cout<<"\n";

// 	glm::vec3 temp;


// 	glm::mat3 R,S;

// 	S = {	{cos(radians),-sin(radians),0},
// 				{sin(radians),cos(radians),0},
// 				{0,0,1}};
// 	if(0==option){		// rotate about up vector

// 		// R = {	{0,my_right_vector[0],my_up_vector[0]},
// 		// 			{0,my_right_vector[1],my_up_vector[1]},
// 		// 			{0,my_right_vector[2],my_up_vector[2]}};

// 		R = {	{1,0,0},
// 					{0,0,1},
// 					{0,1,0}};
// 	}
// 	else{							// rotate about right vector
// 		for(int i=0;i<3;i++){
// 			temp[i] = camPosition[i];
// 		}
// 		temp = glm::normalize(temp);
// 		my_up_vector = glm::normalize(my_up_vector);
// 		my_right_vector = glm::normalize(my_right_vector);

// 		R = {	{-temp[0],my_up_vector[0],my_right_vector[0]},
// 					{-temp[1],my_up_vector[1],my_right_vector[1]},
// 					{-temp[2],my_up_vector[2],my_right_vector[2]}};


// 		// R = {	{temp[0],my_right_vector[0],my_up_vector[0]},
// 		// 			{temp[1],my_right_vector[1],my_up_vector[1]},
// 		// 			{temp[2],my_right_vector[2],my_up_vector[2]}};
// 	}

// 	glm::mat3 res = my_mult(R,my_mult(S,my_get_transpose(R)));


// 	for(int i=0;i<3;i++){
// 		temp[i] = 0;
// 		for(int j=0;j<3;j++){
// 			temp[i]+=res[i][j]*camPosition[j];
// 		}
// 	}
// 	for(int i=0;i<3;i++){
// 		camPosition[i] = temp[i];
// 	}
// 	camPosition = glm::normalize(camPosition);
// 	for(int i=0;i<3;i++) camPosition[i]*=radius;

// 	if(0==option){
// 		for(int i=0;i<3;i++){
// 			temp[i] = 0;
// 			for(int j=0;j<3;j++){
// 				temp[i]+=res[i][j]*my_right_vector[j];
// 			}
// 		}
// 		for(int i=0;i<3;i++){
// 			my_right_vector[i] = temp[i];
// 		}
// 		my_right_vector = glm::normalize(my_right_vector);
// 	}
// 	else{
// 		for(int i=0;i<3;i++){
// 			temp[i] = 0;
// 			for(int j=0;j<3;j++){
// 				temp[i]+=res[i][j]*my_up_vector[j];
// 			}
// 		}
// 		for(int i=0;i<3;i++){
// 			my_up_vector[i] = temp[i];
// 		}
// 		my_up_vector = glm::normalize(my_up_vector);
// 	}

// 	for(int i=0;i<3;i++) temp[i] = camPosition[i];

// 	cout<<"\n\n";
// 	cout<<"dot (temp,up) = "<<my_dot(temp, my_up_vector)<<"\n";
// 	cout<<"dot (temp,right) = "<<my_dot(temp, my_right_vector)<<"\n";
// 	cout<<"dot (right,up) = "<<my_dot(my_right_vector, my_up_vector)<<"\n";
// 	cout<<"l(temp) = "<<glm::length(temp)<<"\n";
// 	cout<<"l(up) = "<<glm::length(my_up_vector)<<"\n";
// 	cout<<"l(right) = "<<glm::length(my_right_vector)<<"\n";
// }
void my_rotate(int option, float radians){

	radians/=40;
	if(0==option){
		theta+=radians;
	}
	else{
		phi+=radians/2;
	}	

	glm::vec3 temp;
	temp[0] = sin(-phi) * cos(-theta) * my_get_radius();
	temp[2] = sin(-phi) * sin(-theta) * my_get_radius();
	temp[1] = cos(phi) * my_get_radius();

	for(int i=0;i<3;i++) camPosition[i] = temp[i];
		camPosition[1]*=-1;

}
int main(int, char**)
{


	// Setup window
	GLFWwindow *window = setupWindow(screen_width, screen_height);
	ImGuiIO& io = ImGui::GetIO(); // Create IO 
	ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f); 
	// camPosition = glm::vec4(20.0, 40.0, 80.0, 1.0);
	// camPosition = glm::vec4(38.637, 20.7055, 66.9213, 1.0);
	camPosition = glm::vec4(44.43, 23.81, 76.96, 1.0);
	my_up_vector = glm::vec3(0.0,1.0,0.0);
	my_right_vector = glm::vec3(1.0,0.0,0.0);
	float radius = my_get_radius();



	unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
	//Get handle to color variable in shader
	vColor_uniform = glGetUniformLocation(shaderProgram, "vColor");
	if(vColor_uniform == -1){
		fprintf(stderr, "Could not bind location: vColor\n");
		exit(0);
	}

	glUseProgram(shaderProgram);

	//setupModelTransformation(shaderProgram);
	// Modelling transformation is setup in the display loop
	setupViewTransformation(shaderProgram);
	setupProjectionTransformation(shaderProgram);

	unsigned int cube_VAO, axis_VAO;
	createCubeObject(shaderProgram, cube_VAO);
	createAxesLine(shaderProgram, axis_VAO);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

				// Get key presses
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))) {
          strcpy(textKeyStatus, "Key status: Left");
          my_rotate(0,-1);
          setupViewTransformation(shaderProgram);
          // setupModelTransformationAxis(shaderProgram, glm::radians(10.0), glm::vec3(1, 0, 0));
        }
        else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow))) {
          strcpy(textKeyStatus, "Key status: Right");
          my_rotate(0,1);
          setupViewTransformation(shaderProgram);
          // setupModelTransformationAxis(shaderProgram, glm::radians(20.0), glm::vec3(1, 0, 0));
        }
        else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
          if(io.KeyShift){
          	strcpy(textKeyStatus, "Key status: Shift + Up");
          	my_scale((my_get_radius() + 1)/my_get_radius());
          	setupViewTransformation(shaderProgram);
          }
          else{
          	strcpy(textKeyStatus, "Key status: Up");
          	// camPosition.y+=1;
          	my_rotate(1,1);
          	setupViewTransformation(shaderProgram);
          }
        }
        else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
          if(io.KeyShift){
          	strcpy(textKeyStatus, "Key status: Shift + Down");
          	my_scale((max(my_get_radius() - 1, (float)0.1))/my_get_radius());
          	setupViewTransformation(shaderProgram);
          }
          else {
            strcpy(textKeyStatus, "Key status: Down");
          	// camPosition.y+=-1;
          	my_rotate(1,-1);
          	setupViewTransformation(shaderProgram);
          }
        }
        else if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Z))){
        	if(!my_is_pressed){
	        		if(!is_perspective){
	        		if(0==my_index){
	        			cout<<"top view\n";
	        			my_up_vector = glm::vec3(1.0,0.0,0.0);
	        		}
	        		else if(1==my_index){
	        			cout<<"front view\n";
	        			my_up_vector = glm::vec3(0.0,1.0,0.0);
	        		}
	        		else if(2==my_index){
	        			cout<<"side view\n";
	        			my_up_vector = glm::vec3(0.0,1.0,0.0);
	        		}
	        		for(int i=0;i<3;i++) camPosition[i] = my_pre_set[my_index][i];
		        	my_index++;
		        	my_index%=3;
		        	setupViewTransformation(shaderProgram);
		        	my_is_pressed = true;
	        	}
        	}
        	else{
        		my_is_pressed = true;
        	}
        }
        else if(!(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Z)))){
        	my_is_pressed = false;
        }
        else 
          strcpy(textKeyStatus, "Key status:");

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glUseProgram(shaderProgram);

		//ImGui UI menu
		ImGui::Begin("Main", NULL, ImGuiWindowFlags_AlwaysAutoResize);                          
		if(ImGui::CollapsingHeader("Information", ImGuiTreeNodeFlags_DefaultOpen))
		{
		  ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("%s", textKeyStatus);
			ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", camPosition.x, camPosition.y, camPosition.z);
		}
		ImGui::End();

		ImGui::Begin("Switch Mode");
		if (ImGui::Button("Toggle")) {
			is_perspective=!is_perspective;
			if(is_perspective) my_up_vector = glm::vec3(0.0,1.0,0.0);
			setupProjectionTransformation(shaderProgram);

    }

		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		setupModelTransformationCube(shaderProgram);
		glBindVertexArray(cube_VAO); 
		glUniform4f(vColor_uniform, 0.5, 0.5, 0.5, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
		glUniform4f(vColor_uniform, 0.0, 0.0, 0.0, 1.0);
		glDrawArrays(GL_LINE_STRIP, 0, 6*2*3);

    glDisable(GL_DEPTH_TEST); // Disable depth test for drawing axes. We want axes to be drawn on top of all

		glBindVertexArray(axis_VAO); 
		setupModelTransformationAxis(shaderProgram, 0.0, glm::vec3(0, 0, 1));
		glUniform4f(vColor_uniform, 1.0, 0.0, 0.0, 1.0); //Red -> X 
		glDrawArrays(GL_LINES, 0, 2);

		setupModelTransformationAxis(shaderProgram, glm::radians(90.0), glm::vec3(0, 0, 1));
		glUniform4f(vColor_uniform, 0.0, 1.0, 0.0, 1.0); //Green -> Y
		glDrawArrays(GL_LINES, 0, 2);

		setupModelTransformationAxis(shaderProgram, -glm::radians(90.0), glm::vec3(0, 1, 0));
		glUniform4f(vColor_uniform, 0.0, 0.0, 1.0, 1.0); //Blue -> Z
		glDrawArrays(GL_LINES, 0, 2);

    glEnable(GL_DEPTH_TEST); // Enable depth test again

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	cleanup(window);

	return 0;
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
	GLfloat cube_vertices[] = {10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10, //Front
		10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10}; //Back
	GLushort cube_indices[] = {
		0, 1, 2, 0, 2, 3, //Front
		4, 7, 5, 5, 7, 6, //Back
		1, 6, 2, 1, 5, 6, //Left
		0, 3, 4, 4, 7, 3, //Right
		0, 4, 1, 4, 5, 1, //Top
		2, 6, 3, 3, 6, 7 //Bottom
	};

	//Generate VAO object
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);

	//Create VBOs for the VAO
	//Position information (data + format)
	int nVertices = (6*2)*3; //(6 faces) * (2 triangles each) * (3 vertices each)
	GLfloat *expanded_vertices = new GLfloat[nVertices*3];
	for(int i=0; i<nVertices; i++) {
		expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
		expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
		expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
	}
	GLuint vertex_VBO;
	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vVertex_attrib);
	glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	delete []expanded_vertices;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void createAxesLine(unsigned int & program, unsigned int &axis_VAO)
{
  glUseProgram(program);

	//Bind shader variables
	int vVertex_attrib_position = glGetAttribLocation(program, "vVertex");
	if(vVertex_attrib_position == -1) {
		fprintf(stderr, "Could not bind location: vVertex\n");
		exit(0);
	}

	//Axes data
	GLfloat axis_vertices[] = {	0, 0, 0, 20, 0, 0}; //X-axis
  glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	//Create VBO for the VAO
	int nVertices = 2; // 2 vertices
	GLuint vertex_VBO;
	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), axis_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vVertex_attrib_position);
	glVertexAttribPointer(vVertex_attrib_position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void setupModelTransformationCube(unsigned int &program)
{
	//Modelling transformations (Model -> World coordinates)
	modelT = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 2.0, 1.0));
	modelT = glm::translate(modelT, glm::vec3(0.0f, 10.0f, 0.0f));

	//Pass on the modelling matrix to the vertex shader
	glUseProgram(program);
	vModel_uniform = glGetUniformLocation(program, "vModel");
	if(vModel_uniform == -1){
		fprintf(stderr, "Could not bind location: vModel\n");
		exit(0);
	}
	glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}

void setupModelTransformationAxis(unsigned int &program, float rot_angle, glm::vec3 rot_axis)
{
	//Modelling transformations (Model -> World coordinates)
	modelT = glm::rotate(glm::mat4(1.0f), rot_angle, rot_axis);

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
	viewT = glm::lookAt(glm::vec3(camPosition), my_up_vector, glm::vec3(my_up_vector));

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
	// projectionT = glm::perspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 0.1f, 1000.0f);

	if(is_perspective){
		projectionT = glm::perspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 0.1f, 1000.0f);
	}
	else{
		float aspect = (GLfloat)screen_width/(GLfloat)screen_height;
		// projectionT = glm::ortho(-aspect * x, aspect * x, -x, x, 0.0f, 200.0f);
		projectionT = glm::ortho(-aspect * 50, aspect * 50, -50.0f, 50.0f, 0.0f, 1000.0f);
	}
	//Pass on the projection matrix to the vertex shader
	glUseProgram(program);
	vProjection_uniform = glGetUniformLocation(program, "vProjection");
	if(vProjection_uniform == -1){
		fprintf(stderr, "Could not bind location: vProjection\n");
		exit(0);
	}
	glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

