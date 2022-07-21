#include "command_pool.h"

vk::raii::CommandPool createCommandPool(const vulkan::Device& device);

namespace vulkan {

	CommandPool::CommandPool(const vulkan::Device& device, uint32_t count) :
		m_pool{ createCommandPool(device) },
		m_buffers{ createBuffers(*device, count) }
	{
	}

	vk::raii::CommandBuffers CommandPool::createBuffers(const vk::raii::Device& device, uint32_t count) {
		
		const vk::CommandBufferAllocateInfo allocate_info{
				.commandPool		= *m_pool,
				.level				= vk::CommandBufferLevel::ePrimary,
				.commandBufferCount = count,
		};

		try {
			return { device, allocate_info };
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to create command buffers\n\t" + std::string(err.what()));
		}

	}

}

vk::raii::CommandPool createCommandPool(const vulkan::Device& device) {

	const vk::CommandPoolCreateInfo pool_info{
			.flags			  = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = device.indices().graphics.value(),
	};

	try {
		return (*device).createCommandPool(pool_info);
	}
	catch (const vk::SystemError& err) {
		throw std::runtime_error("Failed to create command pool\n\t" + std::string(err.what()));
	}

}