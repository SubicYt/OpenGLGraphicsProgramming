#include "MyShaders.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//instantiate a GLFW Window()
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")

unsigned int configure_texture(const char file_name[600]){
	int width, height, nr_channels;
	unsigned int texture_object;
	unsigned char* image_data = stbi_load(file_name, &width, &height, &nr_channels, 0);

	if (!image_data) {
		std::cout << "failed to retrieve image data";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	//finally generate mipmaps
	glGenerateMipmap(texture_object);
	stbi_image_free(image_data);
	return texture_object;
}


unsigned int set_vertex_shader(const char* vertex_shader) {
	unsigned int vertex_shader_object;
	int status; // check compile status

	vertex_shader_object = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_object, 1, &vertex_shader, NULL);
	glCompileShader(vertex_shader_object);
	glGetShaderiv(vertex_shader_object, GL_COMPILE_STATUS, &status);

	if (!status) {
		std::cout << "failed to compile vertex shader";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	return vertex_shader_object;
}

unsigned int set_fragment_shader(const char* fragment_shader) {
	unsigned int fragment_shader_object;
	int status;
	
	fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_object, 1, &fragment_shader, NULL);
	glCompileShader(fragment_shader_object);
	glGetShaderiv(fragment_shader_object, GL_COMPILE_STATUS, &status);
	
	if (!status) {
		std::cout << "failed to compile fragment shader";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	return fragment_shader_object;
}

unsigned int set_shader_program(unsigned int vertex_shader, unsigned int fragment_shader){
	unsigned int shader_program = glCreateProgram();
	int program_status;

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &program_status);
	if (!program_status) {
		std::cout << "FAILED TO LINK SHADERS";
		glfwTerminate();
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return shader_program;
}