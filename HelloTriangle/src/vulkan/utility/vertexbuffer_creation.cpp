#include "vulkan_utility.h"

#include "../vulkan_context.h"


uint32_t findMemoryType(const vk::raii::PhysicalDevice& device, uint32_t type_filter, const vk::MemoryPropertyFlags& properties);


namespace utility {

	vk::raii::Buffer createVertexBuffer(const vk::raii::Device& log_device, const std::vector<Vertex>& vertices) {

		const vk::BufferCreateInfo create_info{
				.size		 = sizeof(vertices[0]) * vertices.size(),
				.usage		 = vk::BufferUsageFlagBits::eVertexBuffer,
				.sharingMode = vk::SharingMode::eExclusive,
		};

		try {
			return log_device.createBuffer(create_info);
		}
		catch (vk::SystemError& err) {
			throw std::runtime_error("Failed to create vertex buffer.\n\t" + std::string(err.what()));
		}

	}

	vk::raii::DeviceMemory allocateVertexBufferMemory(const vk::raii::Device& log_device, const vk::raii::PhysicalDevice& phys_device, 
													  const vk::raii::Buffer& vertex_buffer, const std::vector<Vertex>& vertices) {

		const vk::MemoryRequirements memory_requirements{ vertex_buffer.getMemoryRequirements() };
		const uint32_t memory_type{ findMemoryType(phys_device, memory_requirements.memoryTypeBits,
									vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) };
		const vk::MemoryAllocateInfo alloc_info{
				.allocationSize	 = memory_requirements.size,
				.memoryTypeIndex = memory_type,
		};

		vk::raii::DeviceMemory vertex_buffer_memory{ nullptr };
		try {
			vertex_buffer_memory = log_device.allocateMemory(alloc_info);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to allocate vertex buffer memory.\n\t" + std::string(err.what()));
		}

		vk::BindBufferMemoryInfo memory_info{
				.buffer = *vertex_buffer,
				.memory = *vertex_buffer_memory,
				.memoryOffset = 0,
		};
		try {
			vertex_buffer.bindMemory(*vertex_buffer_memory, 0);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to bind memory.\n\t" + std::string(err.what()));
		}

		size_t buffer_size{ sizeof(vertices[0]) * vertices.size() };
		void* data{ vertex_buffer_memory.mapMemory(0, buffer_size) };
		memcpy(data, vertices.data(), buffer_size);
		vertex_buffer_memory.unmapMemory();

		return vertex_buffer_memory;

	}

}

uint32_t findMemoryType(const vk::raii::PhysicalDevice& device, uint32_t type_filter, const vk::MemoryPropertyFlags& properties) {

	vk::PhysicalDeviceMemoryProperties memory_properties{ device.getMemoryProperties() };
	for (uint32_t i{ 0 }; i < memory_properties.memoryTypeCount; ++i) {
		if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type");

}