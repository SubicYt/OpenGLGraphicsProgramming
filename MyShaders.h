#ifndef MYSHADERS_H
#define MYSHADERS_H
#include <iostream>

	
unsigned int configure_texture(const char file_name[600]);
unsigned int set_vertex_shader(const char* vertex_shader);
unsigned int set_fragment_shader(const char* fragment_shader);
unsigned int set_shader_program(unsigned int vertex_shader, unsigned int fragment_shader);
const char* get_vertex_shader();
const char* get_fragment_shader();
#endif // MYSHADERS_H#pragma once
