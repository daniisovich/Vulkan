#include "application.h"

#include "glfw/init.h"


Application::Application(const AppInfo& info, uint32_t width, uint32_t height) :
	m_window{ glfw::Initializer::get().createWindow(width, height, info.name) },
	m_instance{ info },
	m_surface{ m_window.createSurface(*m_instance) },
	m_device{ m_instance.physicalDevices(), m_surface, m_device_extensions },
	m_queues{ m_device.createQueues() },
	m_swapchain{ m_device, m_surface, m_window.framebufferSize(), m_image_count },
	m_grapics_pipeline{ *m_device, m_swapchain, m_shaders, m_dynamic_states }
{

}

void Application::run() const {

	while (m_window.open()) {

		m_window.update();
	}
}