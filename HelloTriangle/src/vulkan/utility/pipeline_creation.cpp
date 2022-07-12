#include "vulkan_utility.h"

#include <fstream>
#include <string>
#include <iostream>

#include "../vulkan_context.h"



std::vector<uint8_t> readFile(std::string_view filename);
vk::raii::ShaderModule createShaderModule(const std::vector<uint8_t>& code, const vk::raii::Device& device);


namespace utility {

	vk::raii::Pipeline createGraphicsPipeline(const std::vector<ShaderInfo>& shaders, const std::vector<vk::DynamicState>& dynamic_states, 
											  const vk::raii::Device& device, const Swapchain& swapchain, const vk::raii::RenderPass& renderpass) {

		std::vector<vk::raii::ShaderModule> shader_modules;
		shader_modules.reserve(shaders.size());

		std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;
		shader_stages.reserve(shaders.size());

		for (size_t i{ 0 }; i < shaders.size(); ++i) {
			auto shader_code{ readFile(shaders[i].filename)};
			shader_modules.emplace_back(createShaderModule(shader_code, device));
			shader_stages.emplace_back(vk::PipelineShaderStageCreateInfo{
						.stage	= shaders[i].stage,
						.module = *shader_modules[i],
						.pName	= shaders[i].entry_point.data(),
				});
		}

		auto binding_description{ Vertex::bindingDescription() };
		auto attribute_descriptions{ Vertex::attributeDescriptions() };
		vk::PipelineVertexInputStateCreateInfo vertex_input_info{
				.vertexBindingDescriptionCount		= 1,
				.pVertexBindingDescriptions			= &binding_description,
				.vertexAttributeDescriptionCount	= static_cast<uint32_t>(attribute_descriptions.size()),
				.pVertexAttributeDescriptions		= attribute_descriptions.data(),
		};

		vk::PipelineInputAssemblyStateCreateInfo input_assembly{
				.topology = vk::PrimitiveTopology::eTriangleList,
				.primitiveRestartEnable = VK_FALSE,
		};

		vk::Viewport{
				.x			= 0.0f,
				.y			= 0.0f,
				.width		= static_cast<float>(swapchain.extent.width),
				.height		= static_cast<float>(swapchain.extent.height),
				.minDepth	= 0.0f,
				.maxDepth	= 1.0f,
		};

		vk::Rect2D scissor{
				.offset = {0, 0},
				.extent = swapchain.extent,
		};

		vk::PipelineDynamicStateCreateInfo dynamic_state{
				.dynamicStateCount	= static_cast<uint32_t>(dynamic_states.size()),
				.pDynamicStates		= dynamic_states.data(),
		};

		vk::PipelineViewportStateCreateInfo viewport_state{
				.viewportCount	= 1,
				.scissorCount	= 1,
		};

		vk::PipelineRasterizationStateCreateInfo rasterizer{
				.depthClampEnable			= VK_FALSE,
				.rasterizerDiscardEnable	= VK_FALSE,
				.polygonMode				= vk::PolygonMode::eFill,
				.cullMode					= vk::CullModeFlagBits::eBack,
				.frontFace					= vk::FrontFace::eClockwise,
				.depthBiasEnable			= VK_FALSE,
				.lineWidth					= 1.0f,
		};

		vk::PipelineMultisampleStateCreateInfo multisample{
				.rasterizationSamples	= vk::SampleCountFlagBits::e1,
				.sampleShadingEnable	= VK_FALSE,
		};

		vk::PipelineColorBlendAttachmentState color_blend_attachment{
				.blendEnable	= VK_FALSE,
				.colorWriteMask = vk::ColorComponentFlagBits::eR |
								  vk::ColorComponentFlagBits::eG |
								  vk::ColorComponentFlagBits::eB |
								  vk::ColorComponentFlagBits::eA,	
		};

		vk::PipelineColorBlendStateCreateInfo color_blending{
				.logicOpEnable		= VK_FALSE,
				.attachmentCount	= 1,
				.pAttachments		= &color_blend_attachment,
		};

		vk::PipelineLayoutCreateInfo pipeline_layout_info{};

		vk::raii::PipelineLayout pipeline_layout{ nullptr };
		try {
			pipeline_layout = device.createPipelineLayout(pipeline_layout_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

		vk::GraphicsPipelineCreateInfo pipeline_info{
				.stageCount				= static_cast<uint32_t>(shader_stages.size()),
				.pStages				= shader_stages.data(),
				.pVertexInputState		= &vertex_input_info,
				.pInputAssemblyState	= &input_assembly,
				.pViewportState			= &viewport_state,
				.pRasterizationState	= &rasterizer,
				.pMultisampleState		= &multisample,
				.pColorBlendState		= &color_blending,
				.pDynamicState			= &dynamic_state,
				.layout					= *pipeline_layout,
				.renderPass				= *renderpass,
				.subpass				= 0,
		};

		try {
			return device.createGraphicsPipeline(nullptr, pipeline_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}

}

std::vector<uint8_t> readFile(std::string_view filename) {

	std::ifstream file{ filename.data(), std::ios::binary};
	if (!file.is_open())
		throw std::runtime_error("Failed to open file " + std::string(filename));

	return std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});

}

vk::raii::ShaderModule createShaderModule(const std::vector<uint8_t>& code, const vk::raii::Device& device) {

	vk::ShaderModuleCreateInfo create_info{
			.codeSize	= code.size(),
			.pCode		= reinterpret_cast<const uint32_t*>(code.data()),
	};

	try {
		return device.createShaderModule(create_info);
	}
	catch (const vk::SystemError& err) {
		std::cout << err.what() << std::endl;
		return nullptr;
	}

}