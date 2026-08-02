/*
TODO:
TAKES PRECEDENCE!
REWRITE ALL MATRIX CONFIGS IN SEPERATE CPP FILES

after above steps:
	implement meshes
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
#include "CameraMatricies.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

	//INITIAL CAMERA POSITIONS HERE


void Framebuffer_Set_Callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
	char background_texture_file_path[150] = "C:\\Users\\Ethan Denning\\OneDrive\\Desktop\\OpenGLGraphicsProgramming\\planet_2_0.jpg";


	//Setup texture number 1
	unsigned int stone_texture_object = configure_texture(stone_texture_file_path);
	//Setup texture number 2
	unsigned int graffiti_texture_object = configure_texture(graffiti_texture_file_path);
	//setup backgroun texture
	unsigned int background_texture_object = configure_texture(background_texture_file_path);
	
	//background verticies
	float textured_quad_vertices[] = {
		// position      // texcoords
		-1.0f,  1.0f,    0.0f, 1.0f,
		-1.0f, -1.0f,    0.0f, 0.0f,
		1.0f, -1.0f,    1.0f, 0.0f,

		-1.0f,  1.0f,    0.0f, 1.0f,
		1.0f, -1.0f,    1.0f, 0.0f,
		1.0f,  1.0f,    1.0f, 1.0f
	};
	//df
	//main cube object
	float triangle_verticies[] = {
		//first triangle 
		//formatting of the following bytes:
		//first 3 - position, last 2 - texutre coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verticies), triangle_verticies, GL_STATIC_DRAW);
	//^^^Copy our triange_verticies into our GL_ARRAY_BUFFER, set it to a GL_STATIC_DRAW;

	//create "boiler-plate" for rendering a background image
	unsigned int quad_VAO;
	unsigned int quad_VBO;
	glGenVertexArrays(1, &quad_VAO);
	glBindVertexArray(quad_VAO);

	glGenBuffers(1, &quad_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textured_quad_vertices), textured_quad_vertices, GL_STATIC_DRAW);
	
	/*
	transformation data for our object
	*/
	glm::vec3 cubePositions[] = {
		glm::vec3(4.0f, 0.5f, 9.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f),
		glm::vec3(-4.9f, 4.0f, -9.5f),
		glm::vec3(-9.3f, 13.0f, -15.5f),
		glm::vec3(-10.3f, 8.0f, -22.5f)
	};

	//OUT MODEL AND VIEW MATRICES ARE IN THE RENDERLOOP

	//we must create our perspective projection matrix
	glm::mat4 perspective_proj_matrix = glm::mat4(1.0f);
	//width and height are just going to correspond with my viewport
	perspective_proj_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	//first get and configure the vertex shader
	const char* vertex_shader_source_code = get_vertex_shader();
	unsigned int triangles_shader_obj = set_vertex_shader(vertex_shader_source_code);

	//Next we create and configure the fragment shader
	const char* fragment_shader_source_code = get_fragment_shader();
	unsigned int fragment_shader_object = set_fragment_shader(fragment_shader_source_code);

	const char* background_vertex_src_code = get_background_vertex_shader();
	std::cout << " we made it to the background shader but the vertex shader failed";
	unsigned int background_vertex_shader_obj = set_vertex_shader(background_vertex_src_code);
	
	const char* background_fragment_src_code = get_background_fragment_shader();
	unsigned int background_fragment_shader_obj = set_fragment_shader(background_fragment_src_code);

	//Now we must create a shader program and link the shaders
	unsigned int shader_program;
	shader_program = set_shader_program(triangles_shader_obj, fragment_shader_object);

	unsigned int background_shader_program;
	background_shader_program = set_shader_program(background_vertex_shader_obj, background_fragment_shader_obj);

	
	enable_vertexAttrib_ptrs(VAO, VBO); // see MyShaders src file
	//below we manually do (for the background shader) what the above function is doing

	enable_backgroundVertexAttrib_ptrs(quad_VAO, quad_VBO);

	glUseProgram(shader_program); // make sure to specify program used
	unsigned int stone_texture_loc = glGetUniformLocation(shader_program, "our_stone_texture");
	unsigned int graffiti_texture_loc = glGetUniformLocation(shader_program, "our_graffiti_texture");
	glUniform1i(stone_texture_loc, 0); // specify which texture unit belongs to which shader sampler 
	glUniform1i(graffiti_texture_loc, 1); //specify which texture unit belongs to which shader sampler

	glUseProgram(background_shader_program);
	unsigned int background_sky_img_texture_loc = glGetUniformLocation(background_shader_program, "out_background_texture");
	glUniform1i(background_sky_img_texture_loc, 2);


	glUseProgram(shader_program);
	glBindVertexArray(VAO);
	//pass the perspective matrix data to our shader
	unsigned int perspective_matrix_loc = glGetUniformLocation(shader_program, "perspective_proj_matrix");
	glUniformMatrix4fv(perspective_matrix_loc, 1, GL_FALSE, glm::value_ptr(perspective_proj_matrix));

	//main render loop
	while (!glfwWindowShouldClose(window)) {
		get_usr_input(window);
		glfwSetCursorPosCallback(window, mouse_callback_function);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glUseProgram(background_shader_program);
		glBindVertexArray(quad_VAO);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, background_texture_object);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_DEPTH_TEST);
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stone_texture_object);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, graffiti_texture_object);
		 //why do it a second time in our render loop?
		//INCLUDE THIS IN MAIN RENDER LOOP TO UPDATE EACH FRAME
		//*************************************************************************************************************
		//DEFINE CAMERA POSITIONS BELOW


		glm::mat4 view_matrix = glm::lookAt(camera_position,
			camera_position + camera_front_position, camera_up_axis);

		unsigned int view_matrix_location = glGetUniformLocation(shader_program, "view_matrix");
		glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

		for (int i = 0; i < 12; i++) {
			glm::mat4 model_matrix = glm::mat4(1.0f);
			float angle_of_rotation = 33.0f * i; // just to have different object positioned differently around the scene
			model_matrix = glm::translate(model_matrix, cubePositions[i]);

			model_matrix = glm::rotate(model_matrix, (float)glfwGetTime() * angle_of_rotation * 0.023f, glm::vec3(0.0f, 1.40f, 1.0f));
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