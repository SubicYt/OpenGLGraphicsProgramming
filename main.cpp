/*
TODO:
Coordinate Systems
Camera implementations
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MyShaders.h"


//instantiate a GLFW Window()
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")

	/*  NOTTTEEEE::::::
		OpenGL uses data from glViewPort to transform 2D coordinates it processed
		to coordinates on the screen
		a processed point of
		(-0.5, 0.5) would(as its final transformation) be mapped to(200, 450)

		d coordinates are between - 1 and 1.
		::::::: END NOTE
	*/

void Framebuffer_Set_Callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void user_close_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwTerminate();
	}
}

int main() {
	if (!glfwInit()) {
		std::cout << "failed to initizalize glfw()" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "success" << std::endl;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //specify we are using core profile

	//Create Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercise 2", NULL, NULL);
	if (window == NULL) {
		std::cout << "failed to create window" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	
	//before calling any opengl functions we must initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}

	glfwSetFramebufferSizeCallback(window, Framebuffer_Set_Callback); 
	/*
	This function ABOVE sets the framebuffer resize callback of the specified window, 
	which is called when the framebuffer of the specified window is resized.
	*/


	/*
	THE FOLLOWING SPECIFIED SECTION OF CODE IS MEANT FOR LOADING TEXTURES:
	*/
	char stone_texture_file_path[150] = "C:\\Users\\Ethan Denning\\OneDrive\\Desktop\\OpenGLGraphicsProgramming\\stonetiles_003_diff.jpg";
	char graffiti_texture_file_path[150] = "C:\\Users\\Ethan Denning\\OneDrive\\Desktop\\OpenGLGraphicsProgramming\\apocalyse.png";

	//Setup texture number 1
	unsigned int stone_texture_object = configure_texture(stone_texture_file_path);
	//Setup texture number 2
	unsigned int graffiti_texture_object = configure_texture(graffiti_texture_file_path);
	

	float triangle_verticies[] = {
		//first triangle 
		//formatting of the following bytes:
		//first 3 - position, last 2 - texutre coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		- 0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
		//cube
	};
	
	unsigned int VAO; //vector array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //We now binded out vertex array object to be VAO
	
	//Create a Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Assign GL_ARRAY_BUFFER, our VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verticies), &triangle_verticies, GL_STATIC_DRAW); 
	//^^^Copy our triange_verticies into our GL_ARRAY_BUFFER, set it to a GL_STATIC_DRAW;

	//setup our vertex shader


	/*
	transformation data for our object
	*/

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	glm::mat4 transformation = glm::mat4(1.0f);
	transformation = glm::rotate(transformation, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
	transformation = glm::scale(transformation, glm::vec3(1.5, 1.5, 1.5));
	 //model_matrix is included in render loop
	
	//next create a view matrix and move slightly backward to object is visible
	//this means to move the entire scene forwards (move it along the negative z axis)
	glm::mat4 view_matrix = glm::mat4(1.0f);
	view_matrix = glm::translate(view_matrix, glm::vec3(0.0, 0.0, -3.0f));
	
	//finally we must create our perspective projection matrix
	glm::mat4 perspective_proj_matrix = glm::mat4(1.0f);
	//width and height are just going to correspond with my viewport
	perspective_proj_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	//first get and configure the vertex shader
	const char* vertex_shader_source_code = get_vertex_shader();
	unsigned int triangles_shader_obj = set_vertex_shader(vertex_shader_source_code);

	//Next we create and configure the fragment shader
	const char* fragment_shader_source_code = get_fragment_shader();
	unsigned int fragment_shader_object = set_fragment_shader(fragment_shader_source_code);
	 
	//Now we must create a shader program and link the shaders
	unsigned int shader_program;
	shader_program = set_shader_program(triangles_shader_obj, fragment_shader_object);
	

	enable_vertexAttrib_ptrs(); // see MyShaders src file

	glUseProgram(shader_program); // make sure to specify program used
	unsigned int stone_texture_loc = glGetUniformLocation(shader_program, "our_stone_texture");
	unsigned int graffiti_texture_loc = glGetUniformLocation(shader_program, "our_graffiti_texture");

	glUniform1i(stone_texture_loc, 0); // specify which texture unit belongs to which shader sampler 
	glUniform1i(graffiti_texture_loc, 1); //specify which texture unit belongs to which shader sampler 

	//pass transformation matrix to the shader
	//last param is the actual transformation data
	//we do this with glUnifromMatrix4fv
	unsigned int transformation_uniform_location = glGetUniformLocation(shader_program, "rotation_transformation");
	glUniformMatrix4fv(transformation_uniform_location, 1, GL_FALSE, glm::value_ptr(transformation));
	

	unsigned int view_matrix_loc = glGetUniformLocation(shader_program, "view_matrix");
	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	unsigned int perspective_matrix_loc = glGetUniformLocation(shader_program, "perspective_proj_matrix");
	glUniformMatrix4fv(perspective_matrix_loc, 1, GL_FALSE, glm::value_ptr(perspective_proj_matrix));


	while (!glfwWindowShouldClose(window)) {
		user_close_input(window);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stone_texture_object);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, graffiti_texture_object);

		glBindVertexArray(VAO); //why do it a second time in our render loop?
		//INCLUDE THIS IN MAIN RENDER LOOP TO UPDATE EACH FRAME
		//*************************************************************************************************************

		for (int i = 0; i < 10; i++) {
			glm::mat4 model_matrix = glm::mat4(1.0f);
			float angle_of_rotation = 33.0f * i; // just to have different object positioned differently around the scene
			model_matrix = glm::translate(model_matrix, cubePositions[i]);

			model_matrix = glm::rotate(model_matrix, angle_of_rotation * glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));

			unsigned int model_matrix_location = glGetUniformLocation(shader_program, "model_matrix");
			glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//*************************************************************************************************************

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}