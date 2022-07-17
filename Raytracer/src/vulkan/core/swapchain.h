#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "device.h"


namespace vulkan {

	class Swapchain {

	public:

		Swapchain(const Device& device, const vk::raii::SurfaceKHR& surface, const std::pair<uint32_t, uint32_t>& glfw_framebuffer_size, uint32_t image_count);

		inline vk::Format format() const { return m_format.format; }
		inline vk::Extent2D extent() const { return m_extent; }

	private:

		vk::raii::SwapchainKHR			 m_handle;
		vk::SurfaceFormatKHR			 m_format;
		vk::Extent2D					 m_extent;
		std::vector<vk::raii::ImageView> m_image_views;

	};

}