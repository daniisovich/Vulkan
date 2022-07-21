#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "device.h"
#include "swapchain.h"


struct ShaderInfo {
	std::string_view filename;
	vk::ShaderStageFlagBits stage;
	std::string_view entry_point;
};


namespace vulkan {

	class Pipeline {
		
	public:

		Pipeline(const vk::raii::Device& device, const vulkan::Swapchain& swapchain, const std::vector<ShaderInfo>& shaders,
				 const std::vector<vk::DynamicState>& dynamic_states);

		inline const vk::raii::RenderPass& renderpass() const { return m_renderpass; }

	private:

		vk::raii::Pipeline createGraphicsPipeline(const vk::raii::Device& device, const vk::Extent2D& swapchain_extent, const std::vector<ShaderInfo>& shaders,
												  const std::vector<vk::DynamicState>& dynamic_states);

		vk::raii::RenderPass m_renderpass;
		vk::raii::Pipeline	 m_handle;

	};

}