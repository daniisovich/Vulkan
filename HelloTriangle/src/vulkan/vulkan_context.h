#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>
#include <glm/glm.hpp>

#include "../glfw/glfw_window.h"
#include "utility/vulkan_utility.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 color;

	inline static vk::VertexInputBindingDescription bindingDescription() {
		return vk::VertexInputBindingDescription{ 0, sizeof(Vertex), vk::VertexInputRate::eVertex };
	}

	static std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions() {
		const vk::VertexInputAttributeDescription position_description{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) };
		const vk::VertexInputAttributeDescription color_description{ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) };
		return { position_description, color_description };
	}
};


namespace vulkan {
	class Context {

	public:

		Context(const glfw::Window& window, const std::vector<const char*>& req_device_extensions);

		void draw();
		void recordCommandBuffer(const vk::raii::CommandBuffer& command_buffer, uint32_t image_index);
		void wait() const;

		inline void resized(bool val) { m_resized = val; }
		void recreateSwapchain();

	private:

		const uint32_t m_max_concurrent_frames{ 2 };
		uint32_t m_current_frame{ 0 };
		bool m_resized{ false };

		const glfw::Window& m_window;

		const std::vector<Vertex> m_vertices{
			{{ 0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		};

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
		vk::raii::Buffer						m_vertex_buffer;
		vk::raii::DeviceMemory					m_vertex_buffer_memory;
		vk::raii::CommandPool					m_command_pool;
		std::vector<vk::raii::CommandBuffer>	m_command_buffers;

		std::vector<vk::raii::Semaphore>		m_images_available;
		std::vector<vk::raii::Semaphore>		m_renders_finished;
		std::vector<vk::raii::Fence>			m_concurrent_frames;

		
	};
}