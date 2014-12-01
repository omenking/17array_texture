#ifndef __SHADERS__
#define __SHADERS__

#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint load_shaders( char *, char * );
const char* get_shader( const char *filePath, string &shaderProgramText );

#endif
