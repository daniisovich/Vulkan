#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include <string>


struct AppInfo {
	std::string_view name;
	uint32_t		 version;
	uint32_t		 apiVersion;
};

namespace vulkan {

	class Instance {

	public:

		Instance(const AppInfo& info);

		Instance(const Instance&) = delete;
		Instance& operator=(const Instance&) = delete;

		inline const vk::raii::Instance& operator*() const { return m_instance; }
		inline std::vector<vk::raii::PhysicalDevice> physicalDevices() const { return m_instance.enumeratePhysicalDevices(); }

	private:

		bool supported(const std::vector<const char*>& required_extensions, const std::vector<const char*>& required_layers);

		vk::raii::Context				 m_context;
		vk::raii::Instance				 m_instance;
		vk::raii::DebugUtilsMessengerEXT m_messenger;

	};

}