#pragma once

#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>

#include "glfw_context.h"




namespace glfw {

	class Window : public Context {

	public:

		Window(uint32_t width, uint32_t height, std::string_view title);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		std::pair<uint32_t, uint32_t> framebufferSize() const;
		vk::raii::SurfaceKHR createSurface(const vk::raii::Instance& instance) const;

		inline bool shouldClose() const { return glfwWindowShouldClose(m_handle); }
		inline void pollEvents() const { return glfwPollEvents(); }

	private:

		void setCallbacks();

		GLFWwindow* m_handle;

	};

}