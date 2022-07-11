#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include "vulkan/vulkan_raii.hpp"

#include "../glfw/glfw_window.h"
#include "utility/vulkan_utility.h"


class Context {

public:

	Context(const glfw::Window& window, const std::vector<const char*>& req_device_extensions, const std::pair<uint32_t, uint32_t>& glfw_framebuffer_size);

	void draw();
	void recordCommandBuffer(const vk::raii::CommandBuffer& command_buffer, uint32_t image_index);
	void wait() const;

private:

	const uint32_t m_max_concurrent_frames{ 2 };
	uint32_t current_frame{ 0 };

	vk::raii::Context						m_context;
	vk::raii::Instance						m_instance;
	vk::raii::DebugUtilsMessengerEXT		m_debugmessenger{ nullptr };
	vk::raii::SurfaceKHR					m_surface;
	vk::raii::PhysicalDevice				m_pdevice;
	QueueFamilyIndices						m_indices;
	vk::raii::Device						m_ldevice;
	vk::raii::Queue							m_graphics_queue;
	vk::raii::Queue							m_present_queue;
	Swapchain								m_swapchain;
	std::vector<vk::raii::ImageView>		m_swapchain_image_views;
	vk::raii::RenderPass					m_renderpass;
	vk::raii::Pipeline						m_graphics_pipeline;
	std::vector<vk::raii::Framebuffer>		m_framebuffers;
	vk::raii::CommandPool					m_command_pool;
	std::vector<vk::raii::CommandBuffer>	m_command_buffers;

	std::vector<vk::raii::Semaphore>		m_images_available;
	std::vector<vk::raii::Semaphore>		m_renders_finished;
	std::vector<vk::raii::Fence>			m_concurrent_frames;

};
