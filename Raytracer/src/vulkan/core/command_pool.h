#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "device.h"


namespace vulkan {

	class CommandPool {

	public:

		CommandPool(const vulkan::Device& device, uint32_t count);

	private:

		vk::raii::CommandBuffers createBuffers(const vk::raii::Device& device, uint32_t count);

		vk::raii::CommandPool    m_pool;
		vk::raii::CommandBuffers m_buffers;

	};

}