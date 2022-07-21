#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "../../common.h"
#include "device.h"


namespace vulkan {

	class Swapchain {

	public:

		Swapchain(const Device& device, const vk::raii::SurfaceKHR& surface, const Resolution& glfw_framebuffer_size, uint32_t image_count);

		void recreate(const Resolution& framebuffer_size);

		inline vk::Format format() const { return m_format.format; }
		inline vk::Extent2D extent() const { return m_extent; }
		inline const std::vector<vk::raii::ImageView>& imageViews() const { return m_image_views; }

	private:

		void create(const Resolution& glfw_framebuffer_size, uint32_t image_count);

		const vulkan::Device&		m_device;
		const vk::raii::SurfaceKHR& m_surface;

		vk::raii::SwapchainKHR			 m_handle;
		vk::SurfaceFormatKHR			 m_format;
		vk::Extent2D					 m_extent;

		std::vector<vk::raii::ImageView> m_image_views;

	};

}