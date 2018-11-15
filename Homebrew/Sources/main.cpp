#include <iostream>

#include <3ds.h>

int main() {

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	std::cout << "Hello, world!" << std::endl;

	u64 programID{0};

	APT_GetProgramID(&programID);

	std::cout << "Program ID: " << programID << std::endl;

	bool haveToContinue{true};

	// Main loop
	while (haveToContinue) {

		hidScanInput();

		u32 justPressedKet{hidKeysDown()};
		if(justPressedKet & KEY_SELECT) { haveToContinue = false; }

		gspWaitForVBlank();
		gfxSwapBuffers();
	}

	gfxExit();
	return 0;
		
}
