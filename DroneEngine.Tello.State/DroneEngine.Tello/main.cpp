#include "tello.h"

int main() {
	std::cout << "connecting to drone !\n";

	Tello tello;

	if (!tello.BindCommandSocket()) {
		std::cout << "failed to connect and send command!\n";
		return 0;
	}

	if (!tello.BindStateSocket()) {
		std::cout << "failed to connect to get state!\n";
		return 0;
	}

	const auto state = tello.GetState();
	
}