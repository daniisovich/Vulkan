#include "vulkan_utility.h"

#include <iostream>


namespace utility {

	Swapchain createSwapchain(const vk::raii::PhysicalDevice& pdevice, const QueueFamilyIndices& indices, const vk::raii::Device& ldevice, 
											const vk::raii::SurfaceKHR& surface, const std::pair<uint32_t, uint32_t>& framebuffer_size) {

		SwapchainSupportDetails details{ querySwapchainSupport(pdevice, surface) };
		vk::SurfaceFormatKHR format		= chooseSwapSurfaceFormat(details.formats);
		vk::PresentModeKHR present_mode = chooseSwapPresentMode(details.present_modes);
		vk::Extent2D extent				= chooseSwapExtent(details.capabilities, framebuffer_size);

		uint32_t image_count = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount)
			image_count = details.capabilities.maxImageCount;

			
		vk::SharingMode sharing_mode{ vk::SharingMode::eExclusive };
		std::vector<uint32_t> queue_family_indices;

		if (indices.graphics.value() != indices.present.value()) {
			sharing_mode = vk::SharingMode::eConcurrent;
			queue_family_indices = { indices.graphics.value(), indices.present.value() };
		}

		vk::SwapchainCreateInfoKHR create_info{
			.surface				= *surface,
			.minImageCount			= image_count,
			.imageFormat			= format.format,
			.imageColorSpace		= format.colorSpace,
			.imageExtent			= extent,
			.imageArrayLayers		= 1,
			.imageUsage				= vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode		= sharing_mode,
			.queueFamilyIndexCount	= static_cast<uint32_t>(queue_family_indices.size()),
			.preTransform			= details.capabilities.currentTransform,
			.compositeAlpha			= vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode			= present_mode,
			.clipped				= VK_TRUE,
			.oldSwapchain			= nullptr,
		};

		if (queue_family_indices.size() > 0) {
			create_info.setPQueueFamilyIndices(queue_family_indices.data());
		}

		try {
			vk::raii::SwapchainKHR swapchain{ ldevice.createSwapchainKHR(create_info) };
			auto swapchain_images{ getSwapchainImages(swapchain) };
			return { std::move(swapchain), format.format, extent, swapchain_images };
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return { nullptr };
		}

	}

	SwapchainSupportDetails querySwapchainSupport(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface) {

		SwapchainSupportDetails details;
		details.capabilities	= device.getSurfaceCapabilitiesKHR(*surface);
		details.formats			= device.getSurfaceFormatsKHR(*surface);
		details.present_modes	= device.getSurfacePresentModesKHR(*surface);
		return details;

	}

	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats) {

		for (const auto& format : formats) {
			if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
				return format;
			}
		}
		return formats[0];

	}

	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& present_modes) {

		for (const auto& mode : present_modes) {
			if (mode == vk::PresentModeKHR::eMailbox) {
				return mode;
			}
		}
		return vk::PresentModeKHR::eFifo;

	}

	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const std::pair<uint32_t, uint32_t>& glfw_framebuffer_size) {

		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}

		return vk::Extent2D{
			std::clamp(glfw_framebuffer_size.first, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			std::clamp(glfw_framebuffer_size.second, capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
		};

	}

}
