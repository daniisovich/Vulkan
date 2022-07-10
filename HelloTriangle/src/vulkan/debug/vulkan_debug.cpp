#include "vulkan_debug.h"

#if defined(_DEBUG)

	namespace debug {

		void printInstanceExtensions() {
			auto extensions{ vk::enumerateInstanceExtensionProperties() };
			for (const auto& extension : extensions) {
				std::cout << extension.extensionName << "\n";
			}
			std::cout << "\n";
		}

		void printInstanceLayers() {
			auto layers{ vk::enumerateInstanceLayerProperties() };
			for (const auto& layer : layers) {
				std::cout << layer.layerName << "\n";
			}
			std::cout << "\n";
		}

	}

#endif