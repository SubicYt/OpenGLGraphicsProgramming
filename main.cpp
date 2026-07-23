#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

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
void framebuffer_set_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void usr_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwTerminate();
	}
}

int main() {
	if (!glfwInit()) {
		std::cout << "Failure to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//set as core
		
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
	if (window == NULL) {
		std::cout << "WINDOW() FAILURE TO CREATE";
		exit(EXIT_FAILURE);
	}
	//after creating a window we must set the context
	//bind the window to the execution thread
	glfwMakeContextCurrent(window);

	//now before calling any OpenGl funtion pointers
	//we must initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout <<"failed to initialize glad";
		exit(EXIT_FAILURE);
	}
	

	//we must set-up a viewport
	//now we must specify to use that viewport everytime an "event" refreshes in
	glfwSetFramebufferSizeCallback(window, framebuffer_set_callback);

	/*
	Following these steps we will begin a triangle render;
	first we will use GLSL lang. to 
	*/
	float triangle_verticies[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	/*
	With the vertex data defined we’d like to send it as input to the first process of the graphics
	pipeline: the vertex shader.
	This is done by creating memory on the GPU where we store the vertex
	data, configure how OpenGL should interpret the memory and specify how to send the data to the
	graphics card.

	We manage this memory via so called vertex buffer objects (VBO) that can store a large number
	of vertices in the GPU’s memory.
	*/

	//first create a vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Binds the vertex array object with the name VAO;

	unsigned int VBO;
	glGenBuffers(1, &VBO); //generate unique buffer id with glGenBuffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //"Bind" our general buffer to an array buffer (vertex buffer object)
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verticies), &triangle_verticies, GL_STATIC_DRAW);
	//above line copies data from trianlge_verticies into our currently bound GL_ARRAY_BUFFER, buffer (VBO);
	
	/*
	As of now we created and stored vertex data within our graphics card memory,
	this data is managed with our vertex buffer object VBO.

	NEXT: we write vertex and fragment shaders that actually process this data.
	*/
	const char* vertexShader = "#version 330 core \n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f); \n"
		"}\0";
	//We must next create a shader object: again with a reference id
	unsigned int triangle_shader_object;
	triangle_shader_object = glCreateShader(GL_VERTEX_SHADER);
	//Next we bind the shader source code to the shader object
	glShaderSource(triangle_shader_object, 1, &vertexShader, NULL);
	//After creating a shader object and binding its source code, see if the shader actually compiles
	glCompileShader(triangle_shader_object);
	//COMMIT THE SAME STEPS FOR THE FRAGMENT SHADER JUST DIFFERNT GLSL CODE!!
	int success; //used to check for compile time errrors
	glGetShaderiv(triangle_shader_object, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "Failed to compile vertex shader";
		glfwTerminate();
	}


	/*
	Next we create a "fragment shader" used to give pixels data for their final output color
	*/
	const char* FragmentShader = "#version 330 core \n"
		"out vec4 FragmentData;\n"
		"void main()\n"
		"{\n"
		"FragmentData = vec4(1.0f, 0.1f, 0.2f, 1.0f);\n"
		"}\0";
	
	int fragment_success;
	unsigned int fragment_shader_object;
	fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_object, 1, &FragmentShader, NULL);
	glCompileShader(fragment_shader_object);
	
	glGetShaderiv(fragment_shader_object, GL_COMPILE_STATUS, &fragment_success);
	if (!fragment_success) {
		std::cout << "failed to compile fragment shader";
		glfwTerminate();
	}

	//NOW we must link both shader objects to a shader program that we can use for rendering.

	/*
	When linking the shaders into a program it links the outputs of each shader to the inputs of the
	next shader. 
	*/
	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, triangle_shader_object);
	glAttachShader(shader_program, fragment_shader_object);
	glLinkProgram(shader_program);

	//error checking
	int link_success;
	char link_info_log[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &link_success);
	if (!link_success) {
		glGetProgramInfoLog(shader_program, sizeof(link_info_log), NULL, link_info_log);
		std::cout << "shader program failed to link";
		glfwTerminate();
	}

	//we now activate this shader_program using:
	glUseProgram(shader_program);
	//every shader and rendering call after this above call will use shader_program;
	glDeleteShader(triangle_shader_object);
	glDeleteShader(fragment_shader_object); //can delete these now, we dont need them anymore;

	//Now we must specify how OpenGL and our GPU must use our vertex data before rendering (it doesnt know its a triangle bro)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //given vertex attribute location as its argument;

	

	while (!glfwWindowShouldClose(window)) {
		usr_input(window);

		glClearColor(0.1f, 0.2f, 0.3f, 0.4f); //set window color;
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(VAO); //why do it a second time in our render loop?
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;


}