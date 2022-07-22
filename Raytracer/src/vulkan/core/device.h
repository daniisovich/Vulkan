#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "physical_device.h"


namespace vulkan {

	struct Queues {
		vk::raii::Queue graphics;
		vk::raii::Queue present;
	};


	class Device {

	public:

		Device();
		Device(const std::vector<vk::raii::PhysicalDevice>& physical_devices, const vk::raii::SurfaceKHR& surface, 
			   const std::vector<const char*>& required_extensions, uint32_t concurrent_frames);

		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;

		const vk::raii::Device& operator*() const { return m_logical_device; }

		Queues createQueues() const;

		inline vk::Result waitForFences(uint32_t current_frame) const { 
			return m_logical_device.waitForFences(*m_fences[current_frame], VK_TRUE, std::numeric_limits<uint64_t>::max()); 
		}
		inline void resetFences(uint32_t current_frame) { m_logical_device.resetFences(*m_fences[current_frame]); }
		inline void wait() const { m_logical_device.waitIdle(); }
		inline QueueIndices indices() const { return m_physical_device.indices(); }
		inline SwapchainSupport swapchainSupport() const { return m_physical_device.swapchainSupport(); }
		inline vk::PhysicalDeviceMemoryProperties memoryProperties() const { return (*m_physical_device).getMemoryProperties(); }

	private:

		void createFences(uint32_t count);

		PhysicalDevice   m_physical_device;
		vk::raii::Device m_logical_device;

		std::vector<vk::raii::Fence> m_fences;

	};

}