#pragma once

#include "glfw/window.h"
#include "vulkan/core/instance.h"
#include "vulkan/core/device.h"
#include "vulkan/core/swapchain.h"
#include "vulkan/core/pipeline.h"
#include "vulkan/core/framebuffer.h"
#include "model.h"
#include "vulkan/core/command_pool.h"


class Application {

public:

	Application(const AppInfo& info, uint32_t width, uint32_t height);

	void run();
	void onResize();

private:

	const std::vector<const char*> m_device_extensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,

		VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
	};
	const uint32_t m_image_count{ 3 };

	const std::vector<ShaderInfo> m_shaders{
		{ "src/vulkan/shaders/spir-v/vert.spv", vk::ShaderStageFlagBits::eVertex, "main" },
		{ "src/vulkan/shaders/spir-v/frag.spv", vk::ShaderStageFlagBits::eFragment, "main" },
	};
	const std::vector<vk::DynamicState> m_dynamic_states{
		vk::DynamicState::eViewport,
		vk::DynamicState::eScissor,
	};

	const std::vector<glm::vec3> m_vertices{
		{  0.0f, -0.5f,  0.0f },
		{  0.5f,  0.5f,  0.0f },
		{ -0.5f,  0.5f,  0.0f },
	};

	const std::vector<uint32_t> m_indices{
		1, 2, 3,
	};

	const uint32_t m_concurrent_frames_count{ 2 };
	uint32_t m_current_frame{ 0 };


	glfw::Window		 m_window;
	vulkan::Instance	 m_instance;
	vk::raii::SurfaceKHR m_surface;
	vulkan::Device		 m_device;
	vulkan::Queues		 m_queues;
	vulkan::Swapchain	 m_swapchain;
	vulkan::Pipeline	 m_graphics_pipeline;
	vulkan::Framebuffer	 m_framebuffers;
	Model				 m_model;
	vulkan::CommandPool  m_command_pool;

};