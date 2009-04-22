#include "core.hpp"
#include "gameException.hpp"

#if defined(WIN32)
#include "windows.h"
    #if defined(_DEBUG)
        const String CONFIG_FILE = "config/plugins_win_debug.cfg";
    #else
        const String CONFIG_FILE = "config/plugins_win_release.cfg";
    #endif
    INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
    const String CONFIG_FILE = "config/plugins_lin.cfg";
    int main (int argc, char *argv[]) {
#endif
		try {
			Core core(CONFIG_FILE, "config/Ogre.cfg", "ogre.log",
					"config/resources.cfg", true);
			core.run("config/ship.cfg");
		} catch (GameException &e) {
			cout << "GameException: " << e.what() << endl;
		}

		return 0;
	}
