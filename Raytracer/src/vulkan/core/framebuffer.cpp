#include "framebuffer.h"


namespace vulkan {

	Framebuffer::Framebuffer(const vk::raii::Device& device, const vk::raii::RenderPass& renderpass, 
							 const Swapchain& swapchain) :
		m_device{ device },
		m_renderpass{ renderpass }
	{
		recreate(swapchain);
	}

	void Framebuffer::recreate(const Swapchain& swapchain) {
		create(swapchain);
	}

	void Framebuffer::create(const Swapchain& swapchain) {

		m_framebuffers.reserve(swapchain.imageViews().size());
		for (const auto& image_view : swapchain.imageViews()) {

			const vk::FramebufferCreateInfo framebuffer_info{
					.renderPass = *m_renderpass,
					.attachmentCount = 1,
					.pAttachments = &(*image_view),
					.width = swapchain.extent().width,
					.height = swapchain.extent().height,
					.layers = 1,
			};

			try {
				m_framebuffers.emplace_back(m_device.createFramebuffer(framebuffer_info));
			}
			catch (const vk::SystemError& err) {
				throw std::runtime_error("Failed to create Framebuffer\n\t" + std::string(err.what()));
			}

		}

	}

}