#include "vulkan_utility.h"

#include<map>
#include <set>
#include <string>

int32_t deviceSuitability(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface, const std::vector<const char*>& req_extensions);
bool extensionSupport(const vk::raii::PhysicalDevice& device, const std::vector<const char*>& extensions);


namespace utility {

	vk::raii::PhysicalDevice createPDevice(const vk::raii::Instance& instance, const vk::raii::SurfaceKHR& surface, const std::vector<const char*>& req_extensions) {

		auto pdevices{ instance.enumeratePhysicalDevices() };

		std::multimap<uint32_t, vk::raii::PhysicalDevice> suitable_devices;
		for (const auto& device : pdevices) {
			auto suitability{ deviceSuitability(device, surface, req_extensions) };
			if (suitability >= 0) {
				suitable_devices.insert(std::make_pair(suitability, device));
			}
		}

		if (suitable_devices.size() == 0)
			throw std::runtime_error("Failed to find suitable GPU");

		return suitable_devices.rbegin()->second;

	}

	QueueFamilyIndices findQueueFamilies(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface) {

		QueueFamilyIndices indices;

		std::vector<vk::QueueFamilyProperties> queue_families{ device.getQueueFamilyProperties() };
		for (uint32_t i{ 0 }; i < queue_families.size(); ++i) {
			if (queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				indices.graphics = i;
			}

			vk::Bool32 present_support{ device.getSurfaceSupportKHR(i, *surface) };
			if (present_support) {
				indices.present = i;
			}

			if (indices.complete())
				break;
		}

		return indices;

	}

}



int32_t deviceSuitability(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface, const std::vector<const char*>& req_extensions) {

	QueueFamilyIndices indices{ utility::findQueueFamilies(device, surface) };
	if (!indices.complete() || !extensionSupport(device, req_extensions))
		return -1;

	SwapchainSupportDetails swapchain_details{ utility::querySwapchainSupport(device, surface) };
	if (swapchain_details.formats.empty() || swapchain_details.present_modes.empty())
		return -1;

	vk::PhysicalDeviceProperties properties{ device.getProperties() };
	//vk::PhysicalDeviceFeatures   features{ device.getFeatures() };

	int32_t score{ 0 };

	if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
		score += 1000;
	score += properties.limits.maxImageDimension2D;

	return score;

}

bool extensionSupport(const vk::raii::PhysicalDevice& device, const std::vector<const char*>& extensions) {

	std::vector<vk::ExtensionProperties> available_extensions{ device.enumerateDeviceExtensionProperties() };
	std::set<std::string> req_extensions(extensions.begin(), extensions.end());

	for (const auto& extension : available_extensions) {
		req_extensions.erase(extension.extensionName);
	}

	return req_extensions.empty();

}