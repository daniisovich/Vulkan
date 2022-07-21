#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "swapchain.h"


namespace vulkan {

	class Framebuffer {

	public:

		Framebuffer(const vk::raii::Device& device, const vk::raii::RenderPass& renderpass, 
					const Swapchain& swapchain);

		void recreate(const Swapchain& swapchain);

	private:

		void create(const Swapchain& swapchain);

		const vk::raii::Device&		m_device;
		const vk::raii::RenderPass& m_renderpass;

		std::vector<vk::raii::Framebuffer> m_framebuffers;
	};

}