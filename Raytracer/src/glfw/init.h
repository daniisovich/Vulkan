#pragma once

#include "window.h"


namespace glfw {

	class Initializer {

	public:

		static Initializer& get();
		~Initializer();

		Initializer(const Initializer&) = delete;
		Initializer& operator=(const Initializer&) = delete;

		Window createWindow(const WindowParameter& parameter) const;

	private:
		 
		Initializer();

	};

}