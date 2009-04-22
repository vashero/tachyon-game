#include "core.hpp"
#include "gameException.hpp"

#if defined(WIN32)

#include "windows.h"
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else

	int main (int argc, char *argv[]) {
#endif
		try {
			Core core("config/plugins.cfg", "config/Ogre.cfg", "ogre.log",
					"config/resources.cfg", true);
			core.run("config/ship.cfg");
		} catch (GameException &e) {
			cout << "GameException: " << e.what() << endl;
		}

		return 0;
	}
