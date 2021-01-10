#include "tello.h"
#include "tello_state.h"

int main() {
	std::cout << "connecting to drone !\n";

	Tello tello;
	Tello_State telloState;

	if (!tello.Bind()) {
		std::cout << "failed to connect !\n";
		return 0;
	}

	while (true)
	{
		const auto state = tello.GetState();
		if (state.size() > 0) {
			telloState.ShowStatus(state);
		}
	}
}