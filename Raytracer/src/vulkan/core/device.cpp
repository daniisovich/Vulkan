#include "device.h"

#include <set>
#include <iostream>

vulkan::PhysicalDevice choosePhysicalDevice(const std::vector<vk::raii::PhysicalDevice>& physical_devices, const vk::raii::SurfaceKHR& surface,
											  const std::vector<const char*>& required_extensions);


namespace vulkan {

	Device::Device() :
		m_physical_device{},
		m_logical_device{ nullptr }
	{
	}

	Device::Device(const std::vector<vk::raii::PhysicalDevice>& physical_devices, const vk::raii::SurfaceKHR& surface, 
				   const std::vector<const char*>& required_extensions, uint32_t concurrent_frames) :
		m_logical_device{ nullptr }
	{
		m_physical_device = choosePhysicalDevice(physical_devices, surface, required_extensions);
		m_logical_device  = m_physical_device.createLogicalDevice();
		createFences(concurrent_frames);
	}

	Queues Device::createQueues() const {
		try {
			return Queues{
					.graphics = m_logical_device.getQueue(m_physical_device.indices().graphics.value(), 0),
					.present = m_logical_device.getQueue(m_physical_device.indices().present.value(), 0),
			};
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to retrieve queues from logical device\n\t" + std::string(err.what()));
		}
	}

	void Device::createFences(uint32_t count) {

		m_fences.reserve(count);
		const vk::FenceCreateInfo fence_info{
				.flags = vk::FenceCreateFlagBits::eSignaled,
		};

		for (size_t i{ 0 }; i < count; ++i) {
			try {
				m_fences.emplace_back(m_logical_device.createFence(fence_info));
			}
			catch (const vk::SystemError& err) {
				throw std::runtime_error("Failed to create fence\n\t" + std::string(err.what()));
			}
		}

	}

}

vulkan::PhysicalDevice choosePhysicalDevice(const std::vector<vk::raii::PhysicalDevice>& physical_devices, const vk::raii::SurfaceKHR& surface,
	const std::vector<const char*>& required_extensions) {

	for (const auto& physical_device : physical_devices) {
		const vulkan::PhysicalDevice device{ physical_device, surface, required_extensions };
		if (device.suitable()) {
			return device;
		}
	}

	throw std::runtime_error("Failed to find suitable GPU");

}