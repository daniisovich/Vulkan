#include <iostream>

#include <stdexcept>
#include "application.h"


int main() {

	try {
		const Application app{ AppInfo{"Raytracer", VK_MAKE_API_VERSION(0, 1, 0, 0), VK_API_VERSION_1_2}, 800, 600 };
		app.run();
	}
	catch (const std::runtime_error& err) {
		std::cout << "Error: " << err.what() << std::endl;
	}

	return EXIT_SUCCESS;

}