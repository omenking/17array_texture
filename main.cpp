#include <iostream>
using namespace std;
#include <cstdlib>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

GLFWwindow* window;

#define WINDOW_HEIGHT 768
#define WINDOW_WIDTH 1024

#include "common/shaders.h"
#include "common/texture.h"
#include "common/controls.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{

  if (!glfwInit()) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
    exit(EXIT_FAILURE);
  }

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE       , GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ortho", NULL, NULL);
  if (!window) {
		fprintf(stderr, "Failed to create window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetErrorCallback(error_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

  glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
	}
  if (GL_EXT_texture_array){
		fprintf(stderr, "GL_EXT_texture_array\n");
  }

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	GLuint vai; // vertex array id
	glGenVertexArrays(2, &vai);
	glBindVertexArray(vai);

  char vert[] = "shaders/triangles.vert";
  char frag[] = "shaders/triangles.frag";

  GLuint program    = load_shaders(vert,frag);

	GLuint matrix_id  = glGetUniformLocation(program, "MVP");
	GLuint texture_id = glGetUniformLocation(program, "material");

  GLsizei width         = 16;
  GLsizei height        = 16;
  GLsizei layerCount    = 2;
  GLsizei mipLevelCount = 1;

  unsigned char* data = bmp_data("uvtemplate.bmp");
  //unsigned char* data = bmp_data("tiles.bmp");

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);

  //GLubyte texels[32] = 
  //{
       ////Texels for first image.
       //0,   0,   0,   255,
       //255, 0,   0,   255,
       //0,   255, 0,   255,
       //0,   0,   255, 255,
       ////Texels for second image.
       //255, 255, 255, 255,
       //255, 255,   0, 255,
       //0,   255, 255, 255,
       //255, 0,   255, 255,
  //};
  //glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);
  //glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, texels);

  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_BGR, width, height, layerCount);
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_BGR, GL_UNSIGNED_BYTE, data);

  //glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	delete [] data;

	GLuint vbo;
	GLuint uvbo;
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &uvbo);
  // prep tile
  GLfloat vertex[] = {
    -1, 1,0,
     1, 1,0,
     1,-1,0,

     -1, 1,0,
     -1,-1,0,
      1,-1,0,
  };
  GLfloat uv[] = {
    //-1, 1,0,
     //1, 1,0,
     //1,-1,0,

     //-1, 1,0,
     //-1,-1,0,
      //1,-1,0,

    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,

    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, uvbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

  glUseProgram(program);
  while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix       = getViewMatrix();
		glm::mat4 ModelMatrix      = glm::mat4(1.0);
		glm::mat4 MVP              = ProjectionMatrix * ViewMatrix * ModelMatrix;

    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &MVP[0][0]);
    glUniform1i(texture_id, 0);

    // render tile
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, uvbo);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 2*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);



    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);

	glDeleteProgram(program);
	glDeleteVertexArrays(2, &vai);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

static void error_callback(int error, const char* description){
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, 1);
}
