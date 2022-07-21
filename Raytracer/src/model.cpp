#include "model.h"


Model::Model(const vulkan::Device& device, const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices) :
	m_vertices{ vertices },
	m_indices{ indices },
	m_buffer{ device, m_vertices, vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent }
{
}

