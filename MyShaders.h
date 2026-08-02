#ifndef MYSHADERS_H
#define MYSHADERS_H
#include <iostream>


unsigned int configure_texture(const char file_name[600]);
unsigned int set_vertex_shader(const char* vertex_shader);
unsigned int set_fragment_shader(const char* fragment_shader);
unsigned int set_shader_program(unsigned int vertex_shader, unsigned int fragment_shader);
const char* get_vertex_shader();
const char* get_fragment_shader();
const char* get_background_vertex_shader();
const char* get_background_fragment_shader();
void enable_vertexAttrib_ptrs(unsigned int VAO, unsigned int VBO);
void enable_backgroundVertexAttrib_ptrs(unsigned int quad_VAO, unsigned int quad_VBO);
#endif // MYSHADERS_H#pragma once
