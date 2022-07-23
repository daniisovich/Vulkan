#pragma once

#include "vulkan/core/vertex_buffer.h"


struct Vertex {
	glm::vec3 position;

	static vk::VertexInputBindingDescription bindingDescription() {

		const vk::VertexInputBindingDescription binding_description{
				.binding   = 0,
				.stride    = sizeof(Vertex),
				.inputRate = vk::VertexInputRate::eVertex,
		};
		return binding_description;

	}

	static std::array<vk::VertexInputAttributeDescription, 1> attribute_descriptions() {

		const vk::VertexInputAttributeDescription attribute_description{
				.location = 0,
				.binding  = 0,
				.format   = vk::Format::eR32G32B32Sfloat,
				.offset   = offsetof(Vertex, position),
		};
		return { attribute_description };

	}

};


class Model {

public:

	Model(const vulkan::Device& device, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	inline const vk::raii::Buffer& buffer() const { return m_buffer.buffer(); }
	inline uint32_t vertexCount() const { return static_cast<uint32_t>(m_vertices.size()); }
	inline uint32_t indexCount() const { return static_cast<uint32_t>(m_indices.size()); }

private:

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	vulkan::VertexBuffer m_buffer;

};