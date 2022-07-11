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

	std::vector<vk::raii::CommandBuffer> createCommandBuffers(const vk::raii::Device& device, const vk::raii::CommandPool& command_pool, uint32_t count) {

		const vk::CommandBufferAllocateInfo alloc_info{
				.commandPool		= *command_pool,
				.level				= vk::CommandBufferLevel::ePrimary,
				.commandBufferCount = count,
		};

		try {
			return device.allocateCommandBuffers(alloc_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return {};
		}

	}

}