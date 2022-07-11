#include "application.h"


Application::Application(uint32_t width, uint32_t height, std::string_view title) :
	m_window{ width, height, title },
	m_context{ m_window, {VK_KHR_SWAPCHAIN_EXTENSION_NAME }, m_window.framebufferSize() }
{

}

void Application::run() {

	while (!m_window.shouldClose()) {
		m_window.pollEvents();
		m_context.draw();
	}
	m_context.wait();

}