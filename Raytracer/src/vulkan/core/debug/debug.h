#pragma once

#if defined(_DEBUG)

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#include <iostream>


namespace {

	VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT		severity,
			VkDebugUtilsMessageTypeFlagsEXT				type,
			const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
			void*										p_user_data)
	{
		if (!(strcmp(p_callback_data->pMessageIdName, "Loader Message") == 0)) {
			std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(severity)) << "::"
					  << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagBitsEXT>(type)) << "\n"
					  << "\tName\t\t<" << p_callback_data->pMessageIdName << ">\n"
					  << "\tID\t\t<" << p_callback_data->messageIdNumber << ">\n"
					  << p_callback_data->pMessage << "\n\n";
		}
		return VK_FALSE;
	}

}

namespace vulkan {

	inline vk::DebugUtilsMessengerCreateInfoEXT getDebugCreateInfo() {
		return vk::DebugUtilsMessengerCreateInfoEXT{
				.messageSeverity =
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
				.messageType =
					vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
					vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
					vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
				.pfnUserCallback = debug_callback,
		};
	}

}

#endif