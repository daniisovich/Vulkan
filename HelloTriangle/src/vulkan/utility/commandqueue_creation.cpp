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

}