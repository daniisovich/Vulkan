#include "vertex_buffer.h"

#include "../../model.h"


vk::raii::Buffer createBuffer(const vk::raii::Device& device, vk::DeviceSize size, vk::BufferUsageFlags usage);
uint32_t findMemoryType(const vulkan::Device& device, uint32_t filter, const vk::MemoryPropertyFlags properties);


namespace vulkan {

	VertexBuffer::VertexBuffer(const vulkan::Device& device, const std::vector<Vertex>& vertices, 
							   vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) :
		m_handle{ createBuffer(*device, static_cast<vk::DeviceSize>(sizeof(glm::vec3) * vertices.size()), usage)},
		m_memory{ allocateMemory(device, vertices, properties) }
	{
		bindMemory(vertices, 0);
	}

	vk::raii::DeviceMemory VertexBuffer::allocateMemory(const vulkan::Device& device, const std::vector<Vertex>& vertices, vk::MemoryPropertyFlags properties) {

		const vk::MemoryRequirements requirements{ m_handle.getMemoryRequirements() };
		const uint32_t type{ findMemoryType(device, requirements.memoryTypeBits, properties) };
		const vk::MemoryAllocateInfo allocate_info{
				.allocationSize  = requirements.size,
				.memoryTypeIndex = type,
		};

		try {
			return (*device).allocateMemory(allocate_info);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to allocate vertex buffer memory\n\t" + std::string(err.what()));
		}

	}

	void VertexBuffer::bindMemory(const std::vector<Vertex>& vertices, const vk::DeviceSize offset) {

		try {
			m_handle.bindMemory(*m_memory, offset);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to bind memory\n\t" + std::string(err.what()));
		}

		auto buffer_size = static_cast<vk::DeviceSize>(sizeof(glm::vec3) * vertices.size());
		void* data{ m_memory.mapMemory(offset, buffer_size) };
		memcpy(data, vertices.data(), buffer_size);
		m_memory.unmapMemory();

	}

}

vk::raii::Buffer createBuffer(const vk::raii::Device& device, vk::DeviceSize size, vk::BufferUsageFlags usage) {

	const vk::BufferCreateInfo buffer_info{
			.size		 = size,
			.usage		 = usage,
			.sharingMode = vk::SharingMode::eExclusive,
	};

	try {
		return device.createBuffer(buffer_info);
	}
	catch (const vk::SystemError& err) {
		throw std::runtime_error("Failed to create vertex buffer\n\t" + std::string(err.what()));
	}

}

uint32_t findMemoryType(const vulkan::Device& device, uint32_t filter, const vk::MemoryPropertyFlags properties) {

	const vk::PhysicalDeviceMemoryProperties memory_properties{ device.memoryProperties() };
	for (uint32_t i{ 0 }; i < memory_properties.memoryTypeCount; ++i) {
		if ((filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type");

}