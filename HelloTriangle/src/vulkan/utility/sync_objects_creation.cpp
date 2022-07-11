#include "vulkan_utility.h"

#include <iostream>


namespace utility {

	vk::raii::Semaphore createSemaphore(const vk::raii::Device& device) {

		vk::SemaphoreCreateInfo create_info{};
		try {
			return device.createSemaphore(create_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}

	vk::raii::Fence createFence(const vk::raii::Device& device) {

		vk::FenceCreateInfo create_info{
				.flags = vk::FenceCreateFlagBits::eSignaled,
		};

		try {
			return device.createFence(create_info);
		}
		catch (const vk::SystemError& err) {
			std::cout << err.what() << std::endl;
			return nullptr;
		}

	}
}