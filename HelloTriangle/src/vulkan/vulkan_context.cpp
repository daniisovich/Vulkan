#include "vulkan_context.h"

#include "debug/vulkan_debug.h"


Context::Context(const glfw::Window& window, const std::vector<const char*>& req_device_extensions, const std::pair<uint32_t, uint32_t>& glfw_framebuffer_size) :
	m_context{},
	m_instance{ utility::createInstance(m_context) },
	#if defined(_DEBUG)
	m_debugmessenger{ m_instance, debug::getDebugCreateInfo() },
	#endif
	m_surface{ window.createSurface(m_instance) },
	m_pdevice{ utility::createPDevice(m_instance, m_surface, req_device_extensions) },
	m_indices{ utility::findQueueFamilies(m_pdevice, m_surface) },
	m_ldevice{ utility::createLDevice(m_pdevice, m_indices, req_device_extensions) },	
	m_graphics_queue{ utility::createQueueHandle(m_ldevice, m_indices.graphics.value(), 0) },
	m_present_queue{ utility::createQueueHandle(m_ldevice, m_indices.present.value(), 0) },
	m_swapchain{ utility::createSwapchain(m_pdevice, m_indices, m_ldevice, m_surface, glfw_framebuffer_size) },
	m_swapchain_image_views{ utility::createImageViews(m_ldevice, m_swapchain) },
	m_renderpass{ utility::createRenderpass(m_swapchain, m_ldevice) },
	m_graphics_pipeline{ utility::createGraphicsPipeline({{ "src/vulkan/shaders/spir-v/vert.spv", vk::ShaderStageFlagBits::eVertex, "main" }, 
														  { "src/vulkan/shaders/spir-v/frag.spv", vk::ShaderStageFlagBits::eFragment, "main" }},
														 { vk::DynamicState::eViewport, vk::DynamicState::eScissor }, m_ldevice, m_swapchain, m_renderpass) },
	m_framebuffers{ utility::createFramebuffers(m_ldevice, m_swapchain, m_swapchain_image_views, m_renderpass) },
	m_command_pool{ utility::createCommandPool(m_ldevice, m_indices) },
	m_command_buffer{ utility::createCommandBuffer(m_ldevice, m_command_pool) }
{

}
