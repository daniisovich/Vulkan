#pragma once

#include "glfw/window.h"
#include "vulkan/core/instance.h"
#include "vulkan/core/device.h"


class Application {

public:

	Application(const AppInfo& info, uint32_t width, uint32_t height);

	void run() const;

private:

	const std::vector<const char*> m_device_extensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,

		VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
	};

	glfw::Window		 m_window;
	vulkan::Instance	 m_instance;
	vk::raii::SurfaceKHR m_surface;
	vulkan::Device		 m_device;
	vulkan::Queues		 m_queues;
};