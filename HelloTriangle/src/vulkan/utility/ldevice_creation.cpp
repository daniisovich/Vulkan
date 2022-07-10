#include "vulkan_utility.h"

#include <iostream>
#include <set>


namespace utility {

	vk::raii::Device createLDevice(const vk::raii::PhysicalDevice& pdevice, const QueueFamilyIndices& indices, const std::vector<const char*>& req_extensions) {

		std::set<uint32_t> unique_queue_families{ indices.graphics.value(), indices.present.value() };

		const float queue_priority = 1.0f;

		std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
		queue_create_infos.reserve(unique_queue_families.size());
		for (uint32_t queue_family : unique_queue_families) {
			vk::DeviceQueueCreateInfo queue_create_info{
					.queueFamilyIndex	= queue_family,
					.queueCount			= 1,
					.pQueuePriorities	= &queue_priority,
			};
			queue_create_infos.emplace_back(queue_create_info);
		}
			

		vk::PhysicalDeviceFeatures device_features{};
		vk::DeviceCreateInfo create_info{
				.queueCreateInfoCount		= static_cast<uint32_t>(queue_create_infos.size()),
				.pQueueCreateInfos			= queue_create_infos.data(),
				.enabledLayerCount			= 0,
				.enabledExtensionCount		= static_cast<uint32_t>(req_extensions.size()),
				.ppEnabledExtensionNames	= req_extensions.data(),
				.pEnabledFeatures			= &device_features,
		};

		try {
			return vk::raii::Device{ pdevice, create_info };
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}

}
