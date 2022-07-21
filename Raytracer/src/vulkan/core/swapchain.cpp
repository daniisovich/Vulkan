#include "swapchain.h"

#include <iostream>


vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& modes);
vk::Extent2D setExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const Resolution& glfw_framebuffer_size);


namespace vulkan {

	Swapchain::Swapchain(const Device& device, const vk::raii::SurfaceKHR& surface, const Resolution& glfw_framebuffer_size, uint32_t image_count) :
		m_device{ device },
		m_surface{ surface },
		m_handle{ nullptr }
	{
		create(glfw_framebuffer_size, image_count);
	}

	void Swapchain::recreate(const Resolution& framebuffer_size) {

		uint32_t image_count = static_cast<uint32_t>(m_image_views.size());

		m_handle.clear();
		m_image_views.clear();
	
		create(framebuffer_size, image_count);

	}

	void Swapchain::create(const Resolution& glfw_framebuffer_size, uint32_t image_count) {

		SwapchainSupport device_support{ m_device.swapchainSupport() };
		m_format = chooseSurfaceFormat(device_support.formats);
		m_extent = setExtent(device_support.capabilities, glfw_framebuffer_size);
		vk::PresentModeKHR present_mode = choosePresentMode(device_support.present_modes);

		if (image_count < device_support.capabilities.minImageCount) {
			#if defined(_DEBUG)
			std::cerr << "Error: Requested image count for swapchain below minimal image count supported by device." << std::endl
				<< "Image count set to minimal value" << std::endl;
			#endif
			image_count = device_support.capabilities.minImageCount;
		}

		if (device_support.capabilities.maxImageCount > 0 && image_count > device_support.capabilities.maxImageCount) {
			#if defined(_DEBUG)
			std::cerr << "Error: Requested image count for swapchain above maximal image count supported by device." << std::endl
				<< "Image count set to maximal value" << std::endl;
			#endif
			image_count = device_support.capabilities.maxImageCount;
		}

		vk::SharingMode sharing_mode{ vk::SharingMode::eExclusive };

		std::vector<uint32_t> queue_indices;
		QueueIndices indices{ m_device.indices() };
		if (indices.graphics.value() != indices.present.value()) {
			sharing_mode = vk::SharingMode::eConcurrent;
			queue_indices = { indices.graphics.value(), indices.present.value() };
		}

		vk::SwapchainCreateInfoKHR swapchain_info{
				.surface = *m_surface,
				.minImageCount = image_count,
				.imageFormat = m_format.format,
				.imageColorSpace = m_format.colorSpace,
				.imageExtent = m_extent,
				.imageArrayLayers = 1,
				.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
				.imageSharingMode = sharing_mode,
				.preTransform = device_support.capabilities.currentTransform,
				.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
				.presentMode = present_mode,
				.clipped = VK_TRUE,
		};

		if (queue_indices.size() > 0) {
			swapchain_info.queueFamilyIndexCount = static_cast<uint32_t>(queue_indices.size());
			swapchain_info.pQueueFamilyIndices = queue_indices.data();
		}

		try {
			m_handle = (*m_device).createSwapchainKHR(swapchain_info);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to create swapchain\n\t" + std::string(err.what()));
		}

		std::vector<VkImage> swapchain_images = m_handle.getImages();
		m_image_views.reserve(swapchain_images.size());
		for (const auto& image : swapchain_images) {

			const vk::ImageSubresourceRange subresource_range{
					.aspectMask = vk::ImageAspectFlagBits::eColor,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
			};

			const vk::ImageViewCreateInfo image_view_info{
					.image = image,
					.viewType = vk::ImageViewType::e2D,
					.format = m_format.format,
					.subresourceRange = subresource_range,
			};

			try {
				m_image_views.emplace_back((*m_device).createImageView(image_view_info));
			}
			catch (const vk::SystemError& err) {
				throw std::runtime_error("Failed to create image view\n\t" + std::string(err.what()));
			}

		}

	}

}

vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats) {
	for (const auto& format : formats) {
		if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return format;
		}
	}
	return formats[0];
}

vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& modes) {
	for (const auto& mode : modes) {
		if (mode == vk::PresentModeKHR::eMailbox) {
			return mode;
		}
	}
	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D setExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const Resolution& glfw_framebuffer_size) {

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}

	return vk::Extent2D{
			.width  = std::clamp(glfw_framebuffer_size.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			.height = std::clamp(glfw_framebuffer_size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
	};

}