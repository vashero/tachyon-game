#include "coreInputManager.hpp"
#include "inputRouter.hpp"

#include <Ogre.h>
using namespace Ogre;

CoreInputManager::CoreInputManager()
{
	mOis = NULL;
	mMouse = NULL;
	mKeyboard = NULL;
}

CoreInputManager::~CoreInputManager()
{
	if (mOis) {
		OIS::InputManager::destroyInputSystem(mOis);
	}
}

void CoreInputManager::initialize_ois(unsigned long hWnd, InputListener *inputListener)
{
	// Create the param list to initialize OIS
	OIS::ParamList paramList;
	paramList.insert(OIS::ParamList::value_type
			("WINDOW", StringConverter::toString(hWnd)));

	// Initialize OIS
	mOis = OIS::InputManager::createInputSystem(paramList);

	// Initialize the mouse
	mMouse = static_cast<OIS::Mouse*>(mOis->createInputObject(OIS::OISMouse, true));
	mMouse->setEventCallback(inputListener);

	// Initialize the keyboard
	mKeyboard = static_cast<OIS::Keyboard*>
	(mOis->createInputObject(OIS::OISKeyboard, true));
	mKeyboard->setEventCallback(inputListener);
}

void CoreInputManager::capture()
{
	if (mMouse) {
		mMouse->capture();
	}

	if (mKeyboard) {
		mKeyboard->capture();
	}
}
