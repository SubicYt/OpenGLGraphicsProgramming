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
	
	
	/*  NOTTTEEEE::::::
		OpenGL uses data from glViewPort to transform 2D coordinates it processed
		to coordinates on the screen
		a processed point of
		(-0.5, 0.5) would(as its final transformation) be mapped to(200, 450)

		d coordinates are between - 1 and 1.
		::::::: END NOTE
	*/

	//we must set-up a viewport
	//now we must specify to use that viewport everytime an "event" refreshes in
	glfwSetFramebufferSizeCallback(window, framebuffer_set_callback);

	while (!glfwWindowShouldClose(window)) {
		usr_input(window);

		glClearColor(0.1f, 0.2f, 0.3f, 0.4f); //set window color;
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;


}