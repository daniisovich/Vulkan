#include "init.h"

#include <iostream>
#include <stdexcept>


void debugCallback(int error, const char* description);


namespace glfw {

	Initializer& Initializer::get() {
		static Initializer m_handle{};
		return m_handle;
	}

	Initializer::Initializer() {

		#if defined(_DEBUG)
			glfwSetErrorCallback(debugCallback);
		#endif
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW");
		}

	}

	Initializer::~Initializer() {
		glfwTerminate();
	}

	Window Initializer::createWindow(const WindowParameter& parameter) const {
		return Window{ parameter };
	}

}

void debugCallback(int error, const char* description) {
	switch (error) {
	case GLFW_NOT_INITIALIZED: std::cerr << "GLFW not initialized\n\t" << description << std::endl;			break;
	case GLFW_INVALID_ENUM:	   std::cerr << "Invalid enum\n\t" << description << std::endl;					break;
	case GLFW_INVALID_VALUE:   std::cerr << "Invalid value\n\t" << description << std::endl;				break;
	case GLFW_OUT_OF_MEMORY:   std::cerr << "Memory allocation failed\n\t" << description << std::endl;		break;
	case GLFW_API_UNAVAILABLE: std::cerr << "Requested API not available\n\t" << description << std::endl;  break;
	case GLFW_PLATFORM_ERROR:  std::cerr << "Platform specific error\n\t" << description << std::endl;		break;
	default:				   std::cerr << "Unknown error\n\t" << description << std::endl;
	}
}