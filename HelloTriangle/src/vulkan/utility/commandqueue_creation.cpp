#include "vulkan_utility.h"

#include <iostream>


namespace utility {

	vk::raii::CommandPool createCommandPool(const vk::raii::Device& device, const QueueFamilyIndices& indices) {
		
		const vk::CommandPoolCreateInfo create_info{
				.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
				.queueFamilyIndex = indices.graphics.value(),
		};

		try {
			return device.createCommandPool(create_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}

	vk::raii::CommandBuffer createCommandBuffer(const vk::raii::Device& device, const vk::raii::CommandPool& command_pool) {

		const vk::CommandBufferAllocateInfo alloc_info{
				.commandPool		= *command_pool,
				.level				= vk::CommandBufferLevel::ePrimary,
				.commandBufferCount = 1,
		};

		try {
			auto command_buffer{ device.allocateCommandBuffers(alloc_info) };
			return std::move(command_buffer[0]);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}

	void recordCommandBuffer(const vk::raii::CommandBuffer& command_buffer, uint32_t image_index, const vk::raii::RenderPass& renderpass, 
							 const std::vector<vk::raii::Framebuffer>& framebuffers, const Swapchain& swapchain, const vk::raii::Pipeline& graphics_pipeline) {

		const vk::CommandBufferBeginInfo begin_info{};

		try {
			command_buffer.begin(begin_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
		}

		const vk::Rect2D render_area{
				.offset = {0, 0},
				.extent = swapchain.extent,
		};
		const vk::ClearValue clear_color{ std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f} };

		const vk::RenderPassBeginInfo renderpass_info{
				.renderPass			= *renderpass,
				.framebuffer		= *framebuffers[image_index],
				.renderArea			= render_area,
				.clearValueCount	= 1,
				.pClearValues		= &clear_color,
		};

		command_buffer.beginRenderPass(renderpass_info, vk::SubpassContents::eInline);
		command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphics_pipeline);

		const vk::Viewport viewport{
				.x			= 0.0f,
				.y			= 0.0f,
				.width		= static_cast<float>(swapchain.extent.width),
				.height		= static_cast<float>(swapchain.extent.height),
				.minDepth	= 0.0f,
				.maxDepth	= 1.0f,
		};
		command_buffer.setViewport(0, viewport);

		const vk::Rect2D scissor{
				.offset = {0, 0},
				.extent = swapchain.extent,
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

}