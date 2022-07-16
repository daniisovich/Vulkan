#include "instance.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <set>

#include "debug/debug.h"


std::vector<const char*> getRequiredExtensions();


namespace vulkan {

	Instance::Instance(const AppInfo& info) :
		m_context{},
		m_instance{ nullptr },
		m_messenger{ nullptr }
	{

		vk::ApplicationInfo app_info{
				.pApplicationName	= info.name.data(),
				.applicationVersion = info.version,
				.apiVersion			= info.apiVersion,
		};

		std::unique_ptr<vk::DebugUtilsMessengerCreateInfoEXT> debug_info{ nullptr };
		std::vector<const char*> layers;
		std::vector<const char*> extensions = getRequiredExtensions();

		#if defined(_DEBUG)
			debug_info = std::make_unique<vk::DebugUtilsMessengerCreateInfoEXT>(vulkan::getDebugCreateInfo());
			layers.emplace_back("VK_LAYER_KHRONOS_validation");
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		#endif

		if (!supported(layers, extensions)) {
			throw std::runtime_error("Not all layers/extensions supported");
		}

		vk::InstanceCreateInfo instance_info{
				.pNext					 = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(debug_info.get()),
				.pApplicationInfo		 = &app_info,
				.enabledLayerCount		 = static_cast<uint32_t>(layers.size()),
				.ppEnabledLayerNames	 = layers.data(),
				.enabledExtensionCount	 = static_cast<uint32_t>(extensions.size()),
				.ppEnabledExtensionNames = extensions.data(),
		};

		try {
			m_instance = m_context.createInstance(instance_info);
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to create instance\n\t" + std::string(err.what()));
		}

		#if defined(_DEBUG)
			try {
				m_messenger = m_instance.createDebugUtilsMessengerEXT(vulkan::getDebugCreateInfo());
			}
			catch (const vk::SystemError& err) {
				std::cerr << "Warning: Failed to create debug messenger\n\t" << err.what() << std::endl;
			}
		#endif

	}

	bool Instance::supported(const std::vector<const char*>& required_layers, const std::vector<const char*>& required_extensions) {

		std::vector<vk::LayerProperties>	 instance_layers;
		std::vector<vk::ExtensionProperties> instance_extensions;
		try {
			instance_layers		= m_context.enumerateInstanceLayerProperties();
			instance_extensions = m_context.enumerateInstanceExtensionProperties();
		}
		catch (const vk::SystemError& err) {
			throw std::runtime_error("Failed to retrieve instance layers/extensions\n\t" + std::string(err.what()));
		}

		std::set<std::string> req_layers(required_layers.begin(), required_layers.end());
		std::set<std::string> req_extensions(required_extensions.begin(), required_extensions.end());

		for (const auto& layer : instance_layers) {
			req_layers.erase(layer.layerName);
		}

		for (const auto& extension : instance_extensions) {
			req_extensions.erase(extension.extensionName);
		}

		return req_layers.empty() && req_extensions.empty();

	}

}

std::vector<const char*> getRequiredExtensions() {
	uint32_t size{ 0 };
	const char** extensions = glfwGetRequiredInstanceExtensions(&size);
	return std::vector<const char*>(extensions, extensions + size);
}