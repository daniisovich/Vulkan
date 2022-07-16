#include "window.h"

#include <stdexcept>


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


namespace glfw {

	Window::Window(uint32_t width, uint32_t height, std::string_view title) {

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if (!m_handle) {
			throw std::runtime_error("Failed create GLFW window");
		}

		setCallbacks();

	}

	Window::~Window() {
		glfwDestroyWindow(m_handle);
		m_handle = nullptr;
	}

	vk::raii::SurfaceKHR Window::createSurface(const vk::raii::Instance& instance) {

		VkSurfaceKHR surface;
		const VkResult result = glfwCreateWindowSurface(*instance, m_handle, nullptr, &surface);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface");
		}
		return { instance, surface };
	
	}

	void Window::setCallbacks() {
		glfwSetKeyCallback(m_handle, keyCallback);
	}

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

}