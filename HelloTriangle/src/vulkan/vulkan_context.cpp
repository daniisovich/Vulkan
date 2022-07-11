#include "vulkan_context.h"

#include <iostream>
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
	m_command_buffers{ utility::createCommandBuffers(m_ldevice, m_command_pool, m_max_concurrent_frames) },
	m_images_available{ utility::createSemaphores(m_ldevice, m_max_concurrent_frames) },
	m_renders_finished{ utility::createSemaphores(m_ldevice, m_max_concurrent_frames) },
	m_concurrent_frames{ utility::createFences(m_ldevice, m_max_concurrent_frames) }
{

}

void Context::draw() {

	vk::Result result{ m_ldevice.waitForFences(*m_concurrent_frames[current_frame], VK_TRUE, std::numeric_limits<uint64_t>::max())};
	m_ldevice.resetFences(*m_concurrent_frames[current_frame]);
	
	const vk::AcquireNextImageInfoKHR next_image_info{
			.swapchain	= *m_swapchain.handle,
			.timeout	= std::numeric_limits<uint64_t>::max(),
			.semaphore	= *m_images_available[current_frame],
			.fence		= nullptr,
			.deviceMask = 1,
	};
	auto [_, image_index] = m_ldevice.acquireNextImage2KHR(next_image_info);

	m_command_buffers[current_frame].reset();
	recordCommandBuffer(m_command_buffers[current_frame], image_index);

	vk::PipelineStageFlags wait_stages{	vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::SubmitInfo submit_info{
			.waitSemaphoreCount		= 1,
			.pWaitSemaphores		= &(*m_images_available[current_frame]),
			.pWaitDstStageMask		= &wait_stages,
			.commandBufferCount		= 1,
			.pCommandBuffers		= &(*m_command_buffers[current_frame]),
			.signalSemaphoreCount	= 1,
			.pSignalSemaphores		= &(*m_renders_finished[current_frame]),
	};

	try {
		m_graphics_queue.submit(submit_info, *m_concurrent_frames[current_frame]);
	}
	catch (const vk::SystemError& err) {
		std::cout << err.what() << std::endl;
	}

	vk::PresentInfoKHR present_info{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &(*m_renders_finished[current_frame]),
			.swapchainCount = 1,
			.pSwapchains = &(*m_swapchain.handle),
			.pImageIndices = &image_index,
	};

	try {
		result = m_present_queue.presentKHR(present_info);
	}
	catch (const vk::SystemError& err) {
		std::cout << err.what() << std::endl;
	}

	current_frame = (current_frame + 1) % m_max_concurrent_frames;

}

void Context::recordCommandBuffer(const vk::raii::CommandBuffer& command_buffer, uint32_t image_index) {

	const vk::CommandBufferBeginInfo begin_info{};

	try {
		command_buffer.begin(begin_info);
	}
	catch (const vk::SystemError& err) {
		std::cout << err.what() << std::endl;
	}

	const vk::Rect2D render_area{
			.offset = {0, 0},
			.extent = m_swapchain.extent,
	};
	const vk::ClearValue clear_color{ std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f} };

	const vk::RenderPassBeginInfo renderpass_info{
			.renderPass = *m_renderpass,
			.framebuffer = *m_framebuffers[image_index],
			.renderArea = render_area,
			.clearValueCount = 1,
			.pClearValues = &clear_color,
	};

	command_buffer.beginRenderPass(renderpass_info, vk::SubpassContents::eInline);
	command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_graphics_pipeline);

	const vk::Viewport viewport{
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(m_swapchain.extent.width),
			.height = static_cast<float>(m_swapchain.extent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
	};
	command_buffer.setViewport(0, viewport);

	const vk::Rect2D scissor{
			.offset = {0, 0},
			.extent = m_swapchain.extent,
	};
	command_buffer.setScissor(0, scissor);

	command_buffer.draw(3, 1, 0, 0);

	command_buffer.endRenderPass();

	try {
		command_buffer.end();
	}
	catch (const vk::SystemError& err) {
		std::cout << err.what() << std::endl;
	}

}

void Context::wait() const {
	m_ldevice.waitIdle();
}