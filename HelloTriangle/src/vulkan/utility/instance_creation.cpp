#include "vulkan_utility.h"

#include <iostream>
#include <memory>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../debug/vulkan_debug.h"


std::vector<const char*> getRequiredExtensions();
bool supported(const std::vector<const char*>& req_extensions, const std::vector<const char*>& req_layers);


namespace utility {

	vk::raii::Instance createInstance(const vk::raii::Context& context) {

		vk::ApplicationInfo app_info{
				.pApplicationName	= "Hello Triangle",
				.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
				.apiVersion			= VK_API_VERSION_1_0,
		};

		std::unique_ptr<vk::DebugUtilsMessengerCreateInfoEXT> debug_create_info{ nullptr };
		std::vector<const char*> layers;
		auto extensions{ getRequiredExtensions() };


		#if defined(_DEBUG)
		debug_create_info = std::make_unique<vk::DebugUtilsMessengerCreateInfoEXT>(debug::getDebugCreateInfo());
		layers.emplace_back("VK_LAYER_KHRONOS_validation");
		extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		#endif

		if (!supported(extensions, layers))
			throw std::runtime_error("Not all Extensions/Layers supported");

		vk::InstanceCreateInfo create_info{
				.pNext					 = (VkDebugUtilsMessengerCreateInfoEXT*)(debug_create_info.get()),
				.pApplicationInfo		 = &app_info,
				.enabledLayerCount		 = static_cast<uint32_t>(layers.size()),
				.ppEnabledLayerNames	 = layers.data(),
				.enabledExtensionCount	 = static_cast<uint32_t>(extensions.size()),
				.ppEnabledExtensionNames = extensions.data(),
		};

		try {
			return vk::raii::Instance{ context, create_info };
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}
}


std::vector<const char*> getRequiredExtensions() {

	uint32_t extensions_count{ 0 };
	const char** extensions{ glfwGetRequiredInstanceExtensions(&extensions_count) };
	return std::vector<const char*>(extensions, extensions + extensions_count);

}

bool supported(const std::vector<const char*>& req_extensions, const std::vector<const char*>& req_layers) {

	std::vector<vk::ExtensionProperties> extensions{ vk::enumerateInstanceExtensionProperties() };
	std::vector<vk::LayerProperties>	 layers{ vk::enumerateInstanceLayerProperties() };

	for (const char* required_extension : req_extensions) {
		bool found{ false };
		for (const auto& available_extension : extensions) {
			if (std::strcmp(available_extension.extensionName, required_extension) == 0) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}

	for (const char* required_layer : req_layers) {
		bool found{ false };
		for (const auto& available_layer : layers) {
			if (std::strcmp(available_layer.layerName, required_layer) == 0) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}

	return true;

}