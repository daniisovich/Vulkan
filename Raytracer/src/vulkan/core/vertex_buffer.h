#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include <glm/glm.hpp>

#include "device.h"


namespace vulkan {

	class VertexBuffer {

	public:

		VertexBuffer(const vulkan::Device& device, const std::vector<glm::vec3>& vertices,
				     vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);

	private:

		vk::raii::DeviceMemory allocateMemory(const vulkan::Device& device, const std::vector<glm::vec3>& vertices, vk::MemoryPropertyFlags properties);
		void bindMemory(const std::vector<glm::vec3>& vertices, const vk::DeviceSize offset);

		vk::raii::Buffer m_handle;
		vk::raii::DeviceMemory m_memory;

	};

}