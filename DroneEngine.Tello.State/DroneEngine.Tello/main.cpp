#include "tello.h"
#include "tello_state.h"

int main() {
	std::cout << "connecting to drone !\n";

	Tello tello;

	if (!tello.BindCommand()) {
		std::cout << "failed to connect and send command!\n";
		return 0;
	}

	if (!tello.BindState()) {
		std::cout << "failed to connect to get state!\n";
		return 0;
	}

	while (true)
	{
		const auto state = tello.GetState();
		//if (state.size() > 0) {
		//	telloState.ShowStatus(state);
		//}
	}
}