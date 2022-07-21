#pragma once

#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

#include "../common.h"


struct WindowParameter {
	Resolution resolution;
	std::string_view title;
};

namespace glfw {

	class Window {

	public:

		Window() = delete;
		~Window();


		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		vk::raii::SurfaceKHR createSurface(const vk::raii::Instance& instance);
		Resolution framebufferSize() const;

		inline bool open() const { return !glfwWindowShouldClose(m_handle); }
		inline void update() const { glfwPollEvents(); }

	private:

		Window(const WindowParameter& parameter);
		void setCallbacks();

		GLFWwindow* m_handle;

		friend class Initializer;

	};

}