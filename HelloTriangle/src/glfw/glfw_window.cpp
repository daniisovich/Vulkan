#include "glfw_window.h"

#include <stdexcept>

#include "../vulkan/vulkan_context.h"


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void resizeCallback(GLFWwindow* window, int width, int height) {
	auto context{ reinterpret_cast<vulkan::Context*>(glfwGetWindowUserPointer(window)) };
	context->resized(true);
}

namespace glfw {

	Window::Window(uint32_t width, uint32_t height, std::string_view title)	{

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if (!m_handle)
			throw std::runtime_error("Failed to create glfw window");

	}

	Window::~Window() {
		glfwDestroyWindow(m_handle);
	}

	std::pair<uint32_t, uint32_t> Window::framebufferSize() const {
		int width, height;
		glfwGetFramebufferSize(m_handle, &width, &height);
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}

	vk::raii::SurfaceKHR Window::createSurface(const vk::raii::Instance& instance) const {
		
		VkSurfaceKHR surface;
		VkResult result{ glfwCreateWindowSurface(*instance, m_handle, nullptr, &surface) };
		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface");

		return vk::raii::SurfaceKHR{ instance, surface };

	}

	void Window::setCallbacks(vulkan::Context* context) {
		glfwSetWindowUserPointer(m_handle, (void*)&context);
		glfwSetKeyCallback(m_handle, keyCallback);
		glfwSetFramebufferSizeCallback(m_handle, resizeCallback);
	}

}