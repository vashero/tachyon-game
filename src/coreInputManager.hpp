#pragma once

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

class InputListener;

/**
   Initializes and manages input-related resources.
 */
class CoreInputManager
{
public:
	// Methods --------------------

	/** The empty constructor */
	CoreInputManager();

	/** The destructor */
	~CoreInputManager();

	/** Initialize OIS so we can start listening for input
      @param hWnd The window handle
	 */
	void initialize_ois(unsigned long hWnd, InputListener *inputListener);

	/** Capture input from the mouse and keyboard. */
	void capture();

private:
	// Variables --------------------
	/** The OIS object */
	OIS::InputManager *mOis;

	/** The OIS mouse */
	OIS::Mouse *mMouse;

	/** The OIS keyboard */
	OIS::Keyboard *mKeyboard;
};
