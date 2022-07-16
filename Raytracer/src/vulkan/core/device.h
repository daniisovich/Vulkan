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
			   const std::vector<const char*>& required_extensions);

		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;

		Queues createQueues() const;

	private:

		PhysicalDevice   m_physical_device;
		vk::raii::Device m_logical_device;
	};

}