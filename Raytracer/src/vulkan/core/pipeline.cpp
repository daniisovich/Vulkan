#include "pipeline.h"

#include <fstream>


struct Dynamic {
	bool viewport = {};
	bool scissor  = {};
};


vk::raii::RenderPass createRenderpass(const vk::raii::Device& device, const vk::Format& swapchain_format);
std::vector<uint8_t> readFile(std::string_view filename);
vk::raii::ShaderModule createShaderModule(const vk::raii::Device& device, const std::vector<uint8_t>& code);
Dynamic checkDynamicStates(const std::vector<vk::DynamicState>& dynamic_states);


namespace vulkan {

	Pipeline::Pipeline(const vk::raii::Device& device, const vulkan::Swapchain& swapchain, const std::vector<ShaderInfo>& shaders,
					   const std::vector<vk::DynamicState>& dynamic_states) :
		m_renderpass{ createRenderpass(device, swapchain.format()) },
		m_handle{ createGraphicsPipeline(device, swapchain.extent(), shaders, dynamic_states) }
	{
	}

	vk::raii::Pipeline Pipeline::createGraphicsPipeline(const vk::raii::Device& device, const vk::Extent2D& swapchain_extent, const std::vector<ShaderInfo>& shaders,
														const std::vector<vk::DynamicState>& dynamic_states) {

		std::vector<vk::raii::ShaderModule> shader_modules;
		std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;

		shader_modules.reserve(shaders.size());
		shader_stages.reserve(shaders.size());

		for (size_t i{ 0 }; i < shaders.size(); ++i) {

			auto shader_code{ readFile(shaders[i].filename) };
			shader_modules.emplace_back(createShaderModule(device, shader_code));

			const vk::PipelineShaderStageCreateInfo shader_stage_info{
					.stage = shaders[i].stage,
					.module = *shader_modules[i],
					.pName = shaders[i].entry_point.data(),
			};
			shader_stages.emplace_back(shader_stage_info);

		}

		vk::PipelineVertexInputStateCreateInfo vertex_input_info{

		};

		vk::PipelineInputAssemblyStateCreateInfo input_assemply_info{
				.topology = vk::PrimitiveTopology::eTriangleList,
				.primitiveRestartEnable = VK_FALSE,
		};

		vk::PipelineDynamicStateCreateInfo dynamic_info{
				.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
				.pDynamicStates = dynamic_states.data(),
		};

		vk::PipelineViewportStateCreateInfo viewport_info{
				.viewportCount = 1,
				.scissorCount = 1,
		};

		auto [dynamic_viewport, dynamic_scissor] = checkDynamicStates(dynamic_states);

		vk::Viewport viewport;
		if (!dynamic_viewport) {
			viewport = vk::Viewport{
					.x = 0.0f,
					.y = 0.0f,
					.width = static_cast<float>(swapchain_extent.width),
					.height = static_cast<float>(swapchain_extent.height),
					.minDepth = 0.0f,
					.maxDepth = 1.0f,
			};
			viewport_info.pViewports = &viewport;
		}

		vk::Rect2D scissor;
		if (!dynamic_scissor) {
			scissor = vk::Rect2D{
					.offset = {0, 0},
					.extent = swapchain_extent,
			};
			viewport_info.pScissors = &scissor;
		}

		vk::PipelineRasterizationStateCreateInfo rasterization_info{
				.depthClampEnable = VK_FALSE,
				.rasterizerDiscardEnable = VK_FALSE,
				.polygonMode = vk::PolygonMode::eFill,
				.cullMode = vk::CullModeFlagBits::eBack,
				.frontFace = vk::FrontFace::eClockwise,
				.depthBiasEnable = VK_FALSE,
				.lineWidth = 1.0f,
		};

		vk::PipelineMultisampleStateCreateInfo multisample_info{
				.rasterizationSamples = vk::SampleCountFlagBits::e1,
				.sampleShadingEnable = VK_FALSE,
		};

		vk::PipelineColorBlendAttachmentState color_blend_attachment{
				.blendEnable = VK_FALSE,
				.colorWriteMask = vk::ColorComponentFlagBits::eR |
								  vk::ColorComponentFlagBits::eG |
								  vk::ColorComponentFlagBits::eB |
								  vk::ColorComponentFlagBits::eA,
		};

		vk::PipelineColorBlendStateCreateInfo color_blend_info{
				.logicOpEnable = VK_FALSE,
				.attachmentCount = 1,
				.pAttachments = &color_blend_attachment,
		};

		vk::PipelineLayoutCreateInfo pipeline_layout_info{};
		vk::raii::PipelineLayout pipeline_layout{ nullptr };
		try {
			pipeline_layout = device.createPipelineLayout(pipeline_layout_info);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to create pipeline layout\n\t" + std::string(err.what()));
		}

		vk::GraphicsPipelineCreateInfo pipeline_info{
				.stageCount = static_cast<uint32_t>(shader_stages.size()),
				.pStages = shader_stages.data(),
				.pVertexInputState = &vertex_input_info,
				.pInputAssemblyState = &input_assemply_info,
				.pViewportState = &viewport_info,
				.pRasterizationState = &rasterization_info,
				.pMultisampleState = &multisample_info,
				.pColorBlendState = &color_blend_info,
				.pDynamicState = &dynamic_info,
				.layout = *pipeline_layout,
				.renderPass = *m_renderpass,
				.subpass = 0,
		};

		try {
			return device.createGraphicsPipeline(nullptr, pipeline_info);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to create graphics pipeline\n\t" + std::string(err.what()));
		}

	}

}

vk::raii::RenderPass createRenderpass(const vk::raii::Device& device, const vk::Format& swapchain_format) {

	const vk::AttachmentDescription attachment_description{
				.format = swapchain_format,
				.samples = vk::SampleCountFlagBits::e1,
				.loadOp = vk::AttachmentLoadOp::eClear,
				.storeOp = vk::AttachmentStoreOp::eStore,
				.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
				.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
				.initialLayout = vk::ImageLayout::eUndefined,
				.finalLayout = vk::ImageLayout::ePresentSrcKHR,
	};

	const vk::AttachmentReference attachment_reference{
			.attachment = 0,
			.layout = vk::ImageLayout::eAttachmentOptimal,
	};

	const vk::SubpassDescription subpass_description{
			.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
			.colorAttachmentCount = 1,
			.pColorAttachments = &attachment_reference,
	};

	const vk::SubpassDependency subpass_dependency{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
			.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
			.srcAccessMask = vk::AccessFlagBits::eNone,
			.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
	};

	const vk::RenderPassCreateInfo renderpass_info{
			.attachmentCount = 1,
			.pAttachments = &attachment_description,
			.subpassCount = 1,
			.pSubpasses = &subpass_description,
			.dependencyCount = 1,
			.pDependencies = &subpass_dependency,
	};

	try {
		return device.createRenderPass(renderpass_info);
	}
	catch (const vk::SystemError& err) {
		throw std::runtime_error("Failed to create renderpass\n\t" + std::string(err.what()));
	}

}

std::vector<uint8_t> readFile(std::string_view filename) {

	std::ifstream file{ filename.data(), std::ios::binary };
	if (!file.is_open())
		throw std::runtime_error("Failed to open file " + std::string(filename));

	return std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});

}

vk::raii::ShaderModule createShaderModule(const vk::raii::Device& device, const std::vector<uint8_t>& code) {

	const vk::ShaderModuleCreateInfo create_info{
			.codeSize = code.size(),
			.pCode	  = reinterpret_cast<const uint32_t*>(code.data()),
	};

	try {
		return device.createShaderModule(create_info);
	}
	catch (const vk::SystemError& err) {
		throw std::runtime_error("Failed to create shader module\n\t" + std::string(err.what()));
	}

}

Dynamic checkDynamicStates(const std::vector<vk::DynamicState>& dynamic_states) {

	Dynamic dynamic{
		.viewport = false,
		.scissor  = false,
	};

	for (auto dynamic_state : dynamic_states) {
		switch (dynamic_state) {
		case vk::DynamicState::eViewport:	dynamic.viewport = true; break;
		case vk::DynamicState::eScissor:	dynamic.scissor  = true; break;
		}
	}

	return dynamic;

}