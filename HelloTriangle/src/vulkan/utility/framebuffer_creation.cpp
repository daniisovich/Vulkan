#include "vulkan_utility.h"


namespace utility {

	std::vector<vk::raii::Framebuffer> createFramebuffers(const vk::raii::Device& device, const Swapchain& swapchain, const std::vector<vk::raii::ImageView>& image_views, 
															const vk::raii::RenderPass& renderpass) {

		std::vector<vk::raii::Framebuffer> framebuffers;
		framebuffers.reserve(image_views.size());

		for (const auto& image_view : image_views) {

			const vk::FramebufferCreateInfo create_info{
					.renderPass			= *renderpass,
					.attachmentCount	= 1,
					.pAttachments		= &(*image_view),
					.width				= swapchain.extent.width,
					.height				= swapchain.extent.height,
					.layers				= 1,
			};

			try {
				framebuffers.emplace_back(device.createFramebuffer(create_info));
			}
			catch (const vk::SystemError& err) {
				throw std::runtime_error("Failed to create Framebuffer: " + std::string(err.what()));
			}

		}

		return framebuffers;

	}

}