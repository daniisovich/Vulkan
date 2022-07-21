#pragma once

#include "vulkan/core/vertex_buffer.h"


class Model {

public:

	Model(const vulkan::Device& device, const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices);

private:

	std::vector<glm::vec3> m_vertices;
	std::vector<uint32_t> m_indices;

	vulkan::VertexBuffer m_buffer;

};