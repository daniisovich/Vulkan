#pragma once

#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>


namespace glfw {

	class Window {

	public:

		Window() = delete;
		~Window();


		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		vk::raii::SurfaceKHR createSurface(const vk::raii::Instance& instance);

		inline bool open() const { return !glfwWindowShouldClose(m_handle); }
		inline void pollEvents() const { glfwPollEvents(); }

	private:

		Window(uint32_t width, uint32_t height, std::string_view title);
		void setCallbacks();

		GLFWwindow* m_handle;

		friend class Initializer;

	};

}