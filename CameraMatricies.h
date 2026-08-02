#ifndef CAMERAMATRICIES_H
#define CAMERAMATRICIES_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern glm::vec3 camera_position;
extern glm::vec3 camera_front_position;
extern glm::vec3 camera_up_axis;

extern float delta_time;
extern float lastFrame;
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;


void get_usr_input(GLFWwindow* window);

void mouse_callback_function(GLFWwindow* window, double Xpos, double Ypos);

#endif // CAMERAMATRICIES_H

