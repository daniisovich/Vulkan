#include "vulkan_utility.h"

#include <iostream>


namespace utility {

	std::vector<vk::raii::Semaphore> createSemaphores(const vk::raii::Device& device, uint32_t count) {

		std::vector<vk::raii::Semaphore> semaphores;
		semaphores.reserve(count);

		vk::SemaphoreCreateInfo create_info{};
		for (size_t i{ 0 }; i < count; ++i) {
			try {
				semaphores.emplace_back(device.createSemaphore(create_info));
			}
			catch (const vk::SystemError& err) {
				throw std::runtime_error("Failed to create Semaphores " + std::string(err.what()));
			}
		}

		return semaphores;

	}

	std::vector<vk::raii::Fence> createFences(const vk::raii::Device& device, uint32_t count) {

		std::vector<vk::raii::Fence> fences;
		fences.reserve(count);

		vk::FenceCreateInfo create_info{
				.flags = vk::FenceCreateFlagBits::eSignaled,
		};
		for (size_t i{ 0 }; i < count; ++i) {
			try {
				fences.emplace_back(device.createFence(create_info));
			}
			catch (const vk::SystemError& err) {
				throw std::runtime_error("Failed to create fences " + std::string(err.what()));
			}
		}

		return fences;

	}

}