#pragma once

#if defined(_DEBUG)

	#include <iostream>
	#include <string>

	#define VULKAN_HPP_NO_CONSTRUCTORS
	#include <vulkan/vulkan.hpp>


	namespace {

		VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
			void* p_user_data)
		{
			std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(severity)) << "::" 
					  << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagBitsEXT>(type)) << "\t" 
					  << "Name\t\t<" << p_callback_data->pMessageIdName << ">\n\t\t" 
					  << "ID\t\t<" << p_callback_data->messageIdNumber << ">\n\t\t"
					  << "Message\t\t" << p_callback_data->pMessage << "\n";
			return VK_FALSE;
		}

	}

	namespace debug {

		inline vk::DebugUtilsMessengerCreateInfoEXT getDebugCreateInfo() {

			return vk::DebugUtilsMessengerCreateInfoEXT{
				.messageSeverity =
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
				,
				.messageType =
					vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
					vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
					vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
				,
				.pfnUserCallback = debug_callback,
			};

		}

		void printInstanceExtensions();
		void printInstanceLayers();

	}	

#endif