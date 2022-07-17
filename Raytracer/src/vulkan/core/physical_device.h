#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include <optional>


struct QueueIndices {
	std::optional<uint32_t> graphics;
	std::optional<uint32_t> present;

	inline bool complete() const { return graphics.has_value() && present.has_value(); }
};

struct SwapchainSupport {
	vk::SurfaceCapabilitiesKHR		  capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR>	  present_modes;
};


namespace vulkan {

	class PhysicalDevice {

	public:

		PhysicalDevice();
		PhysicalDevice(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface, const std::vector<const char*>& required_extensions);

		bool suitable() const;
		vk::raii::Device createLogicalDevice();

		inline const vk::raii::PhysicalDevice& operator*() const { return m_handle; }
		inline QueueIndices indices() const { return m_indices; }
		inline std::vector<const char*> extensions() const { return m_extensions; }
		inline SwapchainSupport swapchainSupport() const { return m_swapchain_support; }

	private:

		QueueIndices findQueueFamilies(const vk::raii::SurfaceKHR& surface);
		bool supported(const std::vector<const char*>& extensions);
		SwapchainSupport querySwapchainSupport(const vk::raii::SurfaceKHR& surface);

		vk::raii::PhysicalDevice m_handle;
		QueueIndices			 m_indices;

		bool					 m_raytracing;
		bool					 m_extensions_supported;
		std::vector<const char*> m_extensions;

		SwapchainSupport		 m_swapchain_support;

	};

}