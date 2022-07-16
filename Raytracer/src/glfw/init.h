#pragma once

#include "window.h"


namespace glfw {

	class Initializer {

	public:

		static Initializer& get();
		~Initializer();

		Initializer(const Initializer&) = delete;
		Initializer& operator=(const Initializer&) = delete;

		Window createWindow(uint32_t width, uint32_t height, std::string_view title) const;

	private:
		 
		Initializer();

	};

}