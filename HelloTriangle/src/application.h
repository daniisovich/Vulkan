#pragma once

#include "glfw/glfw_window.h"
#include "vulkan/vulkan_context.h"


class Application {

public:

	Application(uint32_t width, uint32_t height, std::string_view title);
	void run();

private:

	glfw::Window m_window;
	vulkan::Context m_context;

};