#include "vulkan_utility.h"

#include <iostream>


namespace utility {

	vk::raii::RenderPass createRenderpass(const Swapchain& swapchain, const vk::raii::Device& device) {

		const vk::AttachmentDescription color_attachment{
				.format			= swapchain.format,
				.samples		= vk::SampleCountFlagBits::e1,
				.loadOp			= vk::AttachmentLoadOp::eClear,
				.storeOp		= vk::AttachmentStoreOp::eStore,
				.stencilLoadOp	= vk::AttachmentLoadOp::eDontCare,
				.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
				.initialLayout	= vk::ImageLayout::eUndefined,
				.finalLayout	= vk::ImageLayout::ePresentSrcKHR,
		};

		const vk::AttachmentReference color_attachment_ref{
				.attachment = 0,
				.layout		= vk::ImageLayout::eAttachmentOptimal,
		};

		const vk::SubpassDescription subpass{
				.pipelineBindPoint		= vk::PipelineBindPoint::eGraphics,
				.colorAttachmentCount	= 1,
				.pColorAttachments		= &color_attachment_ref,
		};

		const vk::SubpassDependency dependency{
				.srcSubpass		= VK_SUBPASS_EXTERNAL,
				.dstSubpass		= 0,
				.srcStageMask	= vk::PipelineStageFlagBits::eColorAttachmentOutput,
				.dstStageMask	= vk::PipelineStageFlagBits::eColorAttachmentOutput,
				.srcAccessMask	= vk::AccessFlagBits::eNone,
				.dstAccessMask	= vk::AccessFlagBits::eColorAttachmentWrite,
		};

		const vk::RenderPassCreateInfo create_info{
				.attachmentCount	= 1,
				.pAttachments		= &color_attachment,
				.subpassCount		= 1,
				.pSubpasses			= &subpass,
				.dependencyCount	= 1,
				.pDependencies		= &dependency,
		};		

		try {
			return device.createRenderPass(create_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}

}