#include "application.h"

#include "glfw/init.h"


Application::Application(const AppInfo& info, uint32_t width, uint32_t height) :
	m_window{ glfw::Initializer::get().createWindow({width, height, info.name}) },
	m_instance{ info },
	m_surface{ m_window.createSurface(*m_instance) },
	m_device{ m_instance.physicalDevices(), m_surface, m_device_extensions },
	m_queues{ m_device.createQueues() },
	m_swapchain{ m_device, m_surface, m_window.framebufferSize(), m_image_count },
	m_graphics_pipeline{ *m_device, m_swapchain, m_shaders, m_dynamic_states },
	m_framebuffers{ *m_device, m_graphics_pipeline.renderpass(), m_swapchain },
	m_model{ m_device, m_vertices, m_indices },
	m_command_pool{ m_device, m_concurrent_frames_count }
{

}

void Application::run() {

	while (m_window.open()) {

		/*
		m_device.waitForFences();
		try {
			m_device.acquireNextImage(m_swapchain);
		}
		catch (const vk::OutOfDateKHRError&) {
			onResize();
			continue;
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to acquire next swapchain image\n\t" + std::string(err.what()));
		}
		m_device.resetFences();
		*/
		m_window.update();
	}
}

void Application::onResize() {

	Resolution glfw_framebuffer = m_window.framebufferSize();
	while (glfw_framebuffer.width == 0 || glfw_framebuffer.height == 0) {
		glfw_framebuffer = m_window.framebufferSize();
		glfwWaitEvents();
	}

	m_device.wait();

	m_swapchain.recreate(glfw_framebuffer);
	m_framebuffers.recreate(m_swapchain);

}