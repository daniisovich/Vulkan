#include "vulkan_utility.h"

#include <iostream>


namespace utility {

	std::vector<vk::raii::ImageView> createImageViews(const vk::raii::Device& device, const Swapchain& swapchain) {

		std::vector<vk::raii::ImageView> image_views;
		image_views.reserve(swapchain.images.size());

		for (const auto& image : swapchain.images) {

			const vk::ImageSubresourceRange subresource_range{
					.aspectMask		= vk::ImageAspectFlagBits::eColor,
					.baseMipLevel	= 0,
					.levelCount		= 1,
					.baseArrayLayer = 0,
					.layerCount		= 1,
			};

			vk::ImageViewCreateInfo create_info{
					.image				= image,
					.viewType			= vk::ImageViewType::e2D,
					.format				= swapchain.format,
					.subresourceRange	= subresource_range,
			};

			try {
				image_views.emplace_back(device.createImageView(create_info));
			}
			catch (const vk::SystemError& err) {
				std::cout << err.what() << std::endl;
			}

		}

		return image_views;

	}

}