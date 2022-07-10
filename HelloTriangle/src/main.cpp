#include <iostream>
#include <stdexcept>

#include "application.h"


int main() {

	try {
		Application app{ 800, 600, "Hello Triangle" };
		app.run();
	}
	catch (const std::runtime_error& error) {
		std::cout << "Error: " << error.what() << std::endl;
	}

	return EXIT_SUCCESS;

}