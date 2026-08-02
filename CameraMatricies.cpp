#include "CameraMatricies.h"
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

glm::vec3 camera_position(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front_position(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up_axis(0.0f, 1.0f, 0.0f);

float delta_time = 0.0f;
float lastFrame = 0.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;

void get_usr_input(GLFWwindow* window) {
	//use time change between frames to calculate what camera speed should be
	float current_frame_time = glfwGetTime();
	delta_time = current_frame_time - lastFrame;
	lastFrame = current_frame_time;

	float camera_speed = 2.5f * delta_time;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera_speed *= 3;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_position += camera_speed * camera_front_position;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_position -= camera_speed * camera_front_position;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_position += glm::normalize(glm::cross(camera_up_axis, camera_front_position) * camera_speed) * 0.07f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_position -= glm::normalize(glm::cross(camera_up_axis, camera_front_position) * camera_speed) * 0.07f;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwTerminate();
	}

}

void mouse_callback_function(GLFWwindow* window, double Xpos, double Ypos){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//now that the mouse is set as our input we must calcualte the pitch and yaw
	double Xoffset = Xpos - lastX;
	double Yoffset = lastY - Ypos;
	 lastX = Xpos;
	 lastY = Ypos;
	
	const float camera_sensitivity = 0.05f;
	Xoffset *= camera_sensitivity;
	Yoffset *= camera_sensitivity;
	
	yaw += Xoffset;
	pitch += Yoffset;
	
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 camera_direction;
	camera_direction.x = cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	camera_direction.y = sin(glm::radians(pitch));
	camera_direction.z = sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

	camera_front_position = glm::normalize(camera_direction);
}