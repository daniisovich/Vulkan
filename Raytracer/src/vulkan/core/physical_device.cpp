#include "physical_device.h"

#include <string>
#include <set>


namespace vulkan {

	PhysicalDevice::PhysicalDevice() :
		m_handle{ nullptr },
		m_indices{},
		m_raytracing{ false },
		m_extensions_supported{ false },
		m_extensions{},
		m_swapchain_support{}
	{
	}

	PhysicalDevice::PhysicalDevice(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface, const std::vector<const char*>& required_extensions) :
		m_handle{ device },
		m_indices{ findQueueFamilies(surface) },
		m_raytracing{ false },
		m_extensions_supported{ supported(required_extensions) },
		m_extensions{ required_extensions },
		m_swapchain_support{ querySwapchainSupport(surface) }
	{
	}

	QueueIndices PhysicalDevice::findQueueFamilies(const vk::raii::SurfaceKHR& surface) {

		QueueIndices indices;
		std::vector<vk::QueueFamilyProperties> queue_families{ m_handle.getQueueFamilyProperties() };

		for (uint32_t i{ 0 }; i < queue_families.size(); ++i) {

			if (queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				indices.graphics = i;
			}

			if (m_handle.getSurfaceSupportKHR(i, *surface)) {
				indices.present = i;
			}

			if (indices.complete())
				break;

		}

		return indices;

	}

	bool PhysicalDevice::supported(const std::vector<const char*>& extensions) {

		std::vector<vk::ExtensionProperties> device_extensions;
		try {
			device_extensions = m_handle.enumerateDeviceExtensionProperties();
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to retrieve device extensions\n\t" + std::string(err.what()));
		}

		for (const char* required_extension : extensions) {

			if (strcmp(required_extension, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) == 0) {
				m_raytracing = true;
			}

			bool found{ false };
			for (const auto& device_extension : device_extensions) {
				if (strcmp(required_extension, device_extension.extensionName) == 0) {
					found = true;
					break;
				}
			}
			if (!found) {
				return false;
			}

		}
		return true;

	}

	SwapchainSupport PhysicalDevice::querySwapchainSupport(const vk::raii::SurfaceKHR& surface) {

		SwapchainSupport support{};
		if (m_extensions_supported) {
			support.capabilities  = m_handle.getSurfaceCapabilitiesKHR(*surface);
			support.formats		  = m_handle.getSurfaceFormatsKHR(*surface);
			support.present_modes = m_handle.getSurfacePresentModesKHR(*surface);
		}
		return support;

	}

	bool PhysicalDevice::suitable() const {

		if (!m_indices.complete() || !m_extensions_supported || m_swapchain_support.formats.empty() ||
			m_swapchain_support.present_modes.empty()) 
		{
			return false;
		}

		return true;

	}

	vk::raii::Device PhysicalDevice::createLogicalDevice() {

		const std::set<uint32_t> unique_queue_families{ m_indices.graphics.value(), m_indices.present.value() };

		const float queue_priority{ 1.0f };
		std::vector<vk::DeviceQueueCreateInfo> queue_infos;
		queue_infos.reserve(unique_queue_families.size());
		for (uint32_t queue_family : unique_queue_families) {
			const vk::DeviceQueueCreateInfo queue_info{
					.queueFamilyIndex = queue_family,
					.queueCount = 1,
					.pQueuePriorities = &queue_priority,
			};
			queue_infos.emplace_back(queue_info);
		}

		std::unique_ptr<vk::PhysicalDeviceRayTracingPipelineFeaturesKHR> raytracing_features{ nullptr };
		if (m_raytracing) {

			vk::PhysicalDeviceVulkan12Features vulkan12_features{
				.descriptorIndexing = VK_TRUE,
				.bufferDeviceAddress = VK_TRUE,
			};

			vk::PhysicalDeviceAccelerationStructureFeaturesKHR accel_features{
					.pNext = &vulkan12_features,
					.accelerationStructure = VK_TRUE,
			};

			const vk::PhysicalDeviceRayTracingPipelineFeaturesKHR rt_pipeline_features{
					.pNext = &accel_features,
					.rayTracingPipeline = VK_TRUE,
			};
			raytracing_features = std::make_unique<vk::PhysicalDeviceRayTracingPipelineFeaturesKHR>(rt_pipeline_features);

		}

		const vk::DeviceCreateInfo device_info{
				.pNext = raytracing_features.get(),
				.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
				.pQueueCreateInfos = queue_infos.data(),
				.enabledLayerCount = 0,
				.enabledExtensionCount = static_cast<uint32_t>(m_extensions.size()),
				.ppEnabledExtensionNames = m_extensions.data(),
		};

		try {
			return m_handle.createDevice(device_info);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to create logical device\n\t" + std::string(err.what()));
		}

	}

	vk::PhysicalDeviceRayTracingPipelinePropertiesKHR PhysicalDevice::raytracingProperties() const {
		auto chain = m_handle.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPipelinePropertiesKHR>();
		return chain.get<vk::PhysicalDeviceRayTracingPipelinePropertiesKHR>();
	}
}