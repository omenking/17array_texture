// Include GLFW
#include <iostream>
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in main.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


#define WINDOW_HEIGHT 768
#define WINDOW_WIDTH  1024
#define MAX_ZOOM      192

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0.0f, 0.0f, -40.0f );
float  horizontalAngle = 3.14f;  // Initial horizontal angle : toward -Z
float  verticalAngle   = 0.0f;   // Initial vertical angle : none
float  initialFoV      = 45.0f;  // Initial Field of View
float  FoV             = initialFoV;
float  speed           = 22.0f;   // 3 units / second
double zoomlevel       = MAX_ZOOM;


void scroll(GLFWwindow* window,double x,double y){
	glm::vec3 direction(
		0.0f, 0.0f, y*0.5f
	);

	zoomlevel += y;
	if (zoomlevel < 0)
    zoomlevel = 0;
	else if (zoomlevel > MAX_ZOOM)
    zoomlevel = MAX_ZOOM;

	if (zoomlevel > 0 && zoomlevel < MAX_ZOOM ){
		position += direction;
	}
	//std::cout << zoomlevel << std::endl;
}

void computeMatricesFromInputs(){


	float speed_ratio = speed/(((2.5 * zoomlevel) + 53) / 133); //Determined my measuring width of square at 32 and 0

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(0, 0, 1);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin( - 3.14f/2.0f),
		0,
		cos( - 3.14f/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	double xpos = 0;
	double ypos = 0;

	glfwGetCursorPos (window, &xpos, &ypos);

	int border_thickness = 50;

	float internal_speed_x = 0;
	float internal_speed_y = 0;

	if (xpos && ypos){
		if(ypos < border_thickness){
			internal_speed_y = (border_thickness - ypos) * speed_ratio / border_thickness;
			position += up * deltaTime * internal_speed_y;
		}

		else if(ypos > WINDOW_HEIGHT - border_thickness){
			internal_speed_y = (border_thickness - (WINDOW_HEIGHT  - ypos))  * speed_ratio / border_thickness;
			position -= up * deltaTime * internal_speed_y;
		}

		if(xpos < border_thickness){
			internal_speed_x = (border_thickness - xpos)  * speed_ratio / border_thickness;
			position -= right * deltaTime * internal_speed_x;
		}

		else if(xpos > WINDOW_WIDTH - border_thickness){
			internal_speed_x = (border_thickness - (WINDOW_WIDTH  - xpos))  * speed_ratio / border_thickness;
			position += right * deltaTime * internal_speed_x;
		}
	}

	//std::cout << internal_speed_x << " " << internal_speed_y << " " << speed << std::endl;
	//std::cout << xpos << " " << ypos << std::endl;

	// Move up
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
	    glfwGetKey(window, GLFW_KEY_W)  == GLFW_PRESS)
		position += up * deltaTime * speed_ratio;
	// Move down
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS ||
	    glfwGetKey( window, GLFW_KEY_S )    == GLFW_PRESS)
		position -= up * deltaTime * speed_ratio;
	// Move right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS ||
	    glfwGetKey( window, GLFW_KEY_D )     == GLFW_PRESS)
		position += right * deltaTime * speed_ratio;
	// Move left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS  ||
	    glfwGetKey( window, GLFW_KEY_A )    == GLFW_PRESS)
		position -= right * deltaTime * speed_ratio;

  glfwSetScrollCallback(window,scroll);

	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 256.0f);
	ViewMatrix       = glm::lookAt(position,position+direction,up); // Camera matrix



	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
