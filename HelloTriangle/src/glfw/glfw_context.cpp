#include "glfw_context.h"

#define GLFW_NO_INCLUDE
#include <GLFW/glfw3.h>
#include <stdexcept>


namespace glfw {

	Context::Context() {
		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");
	}

	Context::~Context() {
		glfwTerminate();
	}

}