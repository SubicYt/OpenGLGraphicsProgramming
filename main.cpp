#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	This function sets the framebuffer resize callback of the specified window, 
	which is called when the framebuffer of the specified window is resized.
	*/

	/*
	Begin OpenGL Triangle rendering here:
	*/


	/*
	THE FOLLOWING SPECIFIED SECTION OF CODE IS MEANT FOR LOADING TEXTURES:
	*/
	int width, height, nr_channels;
	unsigned char* texture_data = stbi_load("C:/Users/Ethan Denning/OneDrive/Desktop/OpenGLGraphicsProgramming/stonetiles_003_diff.jpg", &width, &height, &nr_channels, 0);

	if (!texture_data) {
		std::cout << "failed to load texture data";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	unsigned int stone_texture_object;
	glGenTextures(1, &stone_texture_object);

	//bind our texture to a Gl_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, stone_texture_object);
	//set our parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Fill in our GL_TEXTURE_2D with our image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	//finally generate mipmaps
	glGenerateMipmap(stone_texture_object);
	//we dont need our texture_data anymore
	stbi_image_free(texture_data);


	float triangle_verticies[] = {
		//first triangle 
		//formatting of the following bytes:
		//first 3 - position, second 3 - color, last 2 - texutre coords
		-0.5f, -0.5f, 0.0f, 1.0f, 0.3f, 0.5f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.4f, 0.2f, 0.6f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.7f, 0.9f, 0.0f, 0.0f, 1.0f,
		//Second Triangle (right three float values are rgb values)
		0.5f, -0.5f, 0.0f, 0.2f, 0.6f, 0.9f, 1.0f, 1.0f,
		0.75f, 0.5f, 0.0f, 0.4f, 0.1f, 0.1f, 1.0f, 0.0f,
		0.9f, -0.5f, 0.0f, 0.6f, 1.0f, 0.4f, 0.0f, 1.0f
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
	const char* vertex_shader = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor; \n"
		"layout(location = 2) in vec2 aTexCoords; \n"

		"out vec3 ourColor;\n"
		"out vec2 texture_coordinates; \n"

		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"ourColor = aColor;\n"
		"texture_coordinates = aTexCoords;\n"
		"}\0";

	unsigned int triangles_shader_obj;
	triangles_shader_obj = glCreateShader(GL_VERTEX_SHADER);
	//next bind our GLSL source code to the shader object
	glShaderSource(triangles_shader_obj, 1, &vertex_shader, NULL); //rmr for now set len param to NULl
	//compile our shader
	glCompileShader(triangles_shader_obj);
	//check for a successful shader compile
	int vertex_shader_status;
	glGetShaderiv(triangles_shader_obj, GL_COMPILE_STATUS, &vertex_shader_status);

	if (!vertex_shader_status) {
		std::cout << "vertex shader failed to compile";
		glfwTerminate();
	}

	//Next we create the fragment shader
	
	const char* fragment_shader = "#version 330 core\n"
		"out vec4 fragment_data;\n"
		"in vec3 ourColor;\n"
		"in vec2 texture_coordinates; \n"
		"uniform sampler2D our_texture;\n"
		/*
		 GLSL has a built-in data-type for texture objects called
		a sampler that takes as a postfix the texture type we want e.g. sampler1D, sampler3D or in
		our case sampler2D. We can then add a texture to the fragment shader by simply declaring a
		uniform sampler2D that we later assign our texture to.
		*/
		"void main() \n"
		"{\n"
		"fragment_data = texture(our_texture, texture_coordinates);\n"
		"}\0";

	unsigned int fragment_shader_object;
	fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_object, 1, &fragment_shader, NULL);
	glCompileShader(fragment_shader_object);

	int fragment_shader_status;
	glGetShaderiv(fragment_shader_object, GL_COMPARE_REF_TO_TEXTURE, &fragment_shader_status);
	if (!fragment_shader_status) {
		std::cout << "failed to compile fragment shader";
		glfwTerminate();
	}
	
	//Now we must create a shader program and link the shaders
	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, triangles_shader_obj);
	glAttachShader(shader_program, fragment_shader_object);
	glLinkProgram(shader_program); // links the vertex and fragment shaders

	int program_status;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &program_status);
	if (!program_status) {
		std::cout << "Failed To Link Shaders";
		glfwTerminate();
	}
	//We dont need out shader objects anymore
	glDeleteShader(triangles_shader_obj);
	glDeleteShader(fragment_shader_object);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	while (!glfwWindowShouldClose(window)) {
		user_close_input(window);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shader_program);
		
		glBindTexture(GL_TEXTURE_2D, stone_texture_object);
		glBindVertexArray(VAO); //why do it a second time in our render loop?
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}