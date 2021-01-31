#include "tello.h"

int main() {
	int inputChar = 0;

	std::cout << "connecting to drone !\n";

	Tello tello;

	if (!tello.BindSocketAndConnect()) {
		std::cout << "failed to connect and send command!\n";
		return 0;
	}

	if (!tello.BindStateSocket()) {
		std::cout << "failed to connect to get state!\n";
		return 0;
	}

	puts("enter 1 to see the dron status, z will exit.");

	do {

		inputChar = getchar();

		if (inputChar == '1')
		{
			const auto state = tello.GetState();
		}

	} while (inputChar != 'z');
	
}