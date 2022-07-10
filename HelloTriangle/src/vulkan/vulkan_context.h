#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include "vulkan/vulkan_raii.hpp"

#include "../glfw/glfw_window.h"
#include "utility/vulkan_utility.h"


class Context {

public:

	Context(const glfw::Window& window, const std::vector<const char*>& req_device_extensions, const std::pair<uint32_t, uint32_t>& glfw_framebuffer_size);

private:

	vk::raii::Context					m_context;
	vk::raii::Instance					m_instance;
	vk::raii::DebugUtilsMessengerEXT	m_debugmessenger{ nullptr };
	vk::raii::SurfaceKHR				m_surface;
	vk::raii::PhysicalDevice			m_pdevice;
	QueueFamilyIndices					m_indices;
	vk::raii::Device					m_ldevice;
	vk::raii::Queue						m_graphics_queue;
	vk::raii::Queue						m_present_queue;
	Swapchain							m_swapchain;
	std::vector<vk::raii::ImageView>	m_swapchain_image_views;

};
