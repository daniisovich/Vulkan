#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include <glm/glm.hpp>

#include "device.h"

struct Vertex;


namespace vulkan {

	class VertexBuffer {

	public:

		VertexBuffer(const vulkan::Device& device, const std::vector<Vertex>& vertices,
				     vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);

		inline const vk::raii::Buffer& buffer() const { return m_handle; }

	private:

		vk::raii::DeviceMemory allocateMemory(const vulkan::Device& device, const std::vector<Vertex>& vertices, vk::MemoryPropertyFlags properties);
		void bindMemory(const std::vector<Vertex>& vertices, const vk::DeviceSize offset);

		vk::raii::Buffer m_handle;
		vk::raii::DeviceMemory m_memory;

	};

}