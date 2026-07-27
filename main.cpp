/*
TODO:
implement a texture unit? ----> success
Transformations and Coordinate systems
When chapter 1 is finished start optimizing the structure of this code base
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
		//first 3 - position, second 3 - color, last 2 - texutre coords
		-0.5f, -0.5f, 0.0f, 1.0f, 0.3f, 0.5f, 0.0f, 0.0f,//bottom left
		-0.5f, 0.5f, 0.0f, 0.4f, 0.2f, 0.6f, 0.0f, 1.0f, //top left
		0.5f, -0.5f, 0.0f, 0.7f, 0.9f, 0.6f, 1.0f, 0.0f, // bottom right 

		-0.5f, 0.5f, 0.0f, 0.7f, 0.9f, 0.6f, 0.0f, 1.0f, //top left
		0.5f, 0.5f, 0.0f, 0.4f, 0.7f, 0.8f, 1.0f, 1.0f, //top right
		0.5f, -0.5f, 0.0f, 0.7f, 0.9f, 0.6f, 1.0f, 0.0f // bottom right
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
	glm::mat4 transformation = glm::mat4(1.0f);
	transformation = glm::rotate(transformation, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
	transformation = glm::scale(transformation, glm::vec3(1.5, 1.5, 1.5));

	const char* vertex_shader_source_code = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor; \n"
		"layout (location = 2) in vec2 aTexCoordinates; \n"

		"out vec3 ourColor;\n"
		"out vec2 texture_coordinates;\n"
		"uniform mat4 rotation_transformation;\n"//4x4 matrix uniform used for a transformation
		"void main()\n"
		"{\n"
		"gl_Position = rotation_transformation * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n" //90 degree transformation of the positoin
		"ourColor = aColor;\n"
		"texture_coordinates = aTexCoordinates;"
		"}\0";

	unsigned int triangles_shader_obj = set_vertex_shader(vertex_shader_source_code);

	//Next we create the fragment shader
	
	const char* fragment_shader_source_code = "#version 330 core\n"
		"out vec4 fragment_data;\n"
		"in vec3 ourColor;\n"
		"in vec2 texture_coordinates; \n"


		"uniform sampler2D our_stone_texture;\n"
		"uniform sampler2D our_graffiti_texture; \n"
		/*
		 GLSL has a built-in data-type for texture objects called
		a sampler that takes as a postfix the texture type we want e.g. sampler1D, sampler3D or in
		our case sampler2D. We can then add a texture to the fragment shader by simply declaring a
		uniform sampler2D that we later assign our texture to.
		*/
		"void main() \n"
		"{\n"
		"fragment_data = mix(texture(our_stone_texture, texture_coordinates),texture(our_graffiti_texture, texture_coordinates), 0.35);\n"
		"}\0";

	unsigned int fragment_shader_object = set_fragment_shader(fragment_shader_source_code);
	
	//Now we must create a shader program and link the shaders
	unsigned int shader_program;
	shader_program = set_shader_program(triangles_shader_obj, fragment_shader_object);
	

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glUseProgram(shader_program); // make sure to specify program used
	glUniform1i(glGetUniformLocation(shader_program, "our_stone_texture"), 0); // specify which texture unit belongs to which shader sampler 
	glUniform1i(glGetUniformLocation(shader_program, "our_graffiti_texture"), 1); //specify which texture unit belongs to which shader sampler 

	//pass transformation matrix to the shader
	//last param is the actual transformation data
	//we do this with glUnifromMatrix4fv
	unsigned int transformation_uniform_location = glGetUniformLocation(shader_program, "rotation_transformation");
	glUniformMatrix4fv(transformation_uniform_location, 1, GL_FALSE, glm::value_ptr(transformation));
	
	while (!glfwWindowShouldClose(window)) {
		user_close_input(window);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stone_texture_object);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, graffiti_texture_object);
		stbi_set_flip_vertically_on_load(true);

		glBindVertexArray(VAO); //why do it a second time in our render loop?
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}