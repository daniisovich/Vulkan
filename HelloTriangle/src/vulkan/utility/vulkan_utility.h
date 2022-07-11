#pragma once

#include <optional>

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>


struct QueueFamilyIndices {
	std::optional<uint32_t> graphics;
	std::optional<uint32_t> present;

	inline bool complete() const { return graphics.has_value() && present.has_value(); }
};

struct SwapchainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> present_modes;
};

struct Swapchain {
	vk::raii::SwapchainKHR handle;
	vk::Format format;
	vk::Extent2D extent;
	std::vector<vk::Image> images;
};

struct ShaderInfo {
	std::string_view filename;
	vk::ShaderStageFlagBits stage;
	std::string_view entry_point;
};


namespace utility {


	vk::raii::Instance createInstance(const vk::raii::Context& context);
	vk::raii::PhysicalDevice createPDevice(const vk::raii::Instance& instance, const vk::raii::SurfaceKHR& surface, const std::vector<const char*>& req_extensions);
	vk::raii::Device createLDevice(const vk::raii::PhysicalDevice& pdevice, const QueueFamilyIndices& indices, const std::vector<const char*>& req_extensions);
	Swapchain createSwapchain(const vk::raii::PhysicalDevice& pdevice, const QueueFamilyIndices& indices, const vk::raii::Device& ldevice,
											const vk::raii::SurfaceKHR& surface, const std::pair<uint32_t, uint32_t>& framebuffer_size);
	std::vector<vk::raii::ImageView> createImageViews(const vk::raii::Device& device, const Swapchain& swapchain);
	vk::raii::RenderPass createRenderpass(const Swapchain& swapchain, const vk::raii::Device& device);
	vk::raii::Pipeline createGraphicsPipeline(const std::vector<ShaderInfo>& shaders, const std::vector<vk::DynamicState>& dynamic_states,
		const vk::raii::Device& device, const Swapchain& swapchain, const vk::raii::RenderPass& renderpass);
	std::vector<vk::raii::Framebuffer> createFramebuffers(const vk::raii::Device& device, const Swapchain& swapchain, const std::vector<vk::raii::ImageView>& image_views,
		const vk::raii::RenderPass& renderpass);
	vk::raii::CommandPool createCommandPool(const vk::raii::Device& device, const QueueFamilyIndices& indices);
	std::vector<vk::raii::CommandBuffer> createCommandBuffers(const vk::raii::Device& device, const vk::raii::CommandPool& command_pool, uint32_t count);
	std::vector<vk::raii::Semaphore> createSemaphores(const vk::raii::Device& device, uint32_t count);
	std::vector<vk::raii::Fence> createFences(const vk::raii::Device& device, uint32_t count);

	inline vk::raii::Queue createQueueHandle(const vk::raii::Device& device, uint32_t index, uint32_t offset = 0) {
		return device.getQueue(index, offset);
	}

	QueueFamilyIndices findQueueFamilies(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface);
	SwapchainSupportDetails querySwapchainSupport(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface);

	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& present_modes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const std::pair<uint32_t, uint32_t>& glfw_framebuffer_size);

	inline std::vector<vk::Image> getSwapchainImages(const vk::raii::SwapchainKHR& swapchain) { 
		auto images{ swapchain.getImages() };
		return std::vector<vk::Image>(images.begin(), images.end());
	}

}
