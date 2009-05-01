/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#include "core.hpp"
#include "gameException.hpp"

#include "mainMenu.hpp"

Core::Core(const String &pluginFilename, const String &configFilename,
		const String &logFilename, const String &resourceFilename,
		bool showConfigDialog)
{
	mRoot = NULL;
	mWin = NULL;
	mGameWindowEventListener = NULL;

	mStateMachine.request_state_change(CORE_STARTUP);
	mInputRouter.set_state_machine(&mStateMachine);
	mFrameRouter.set_state_machine(&mStateMachine);

	// Initialise all ogre-related objects
	this->initialize_root(pluginFilename, configFilename, logFilename,
			showConfigDialog);

	// Load resources we need
	this->load_ogre_resources(resourceFilename);

	// Initialise the input manager
	unsigned long hWnd;
	mWin->getCustomAttribute("WINDOW", &hWnd);
	mInputManager.initialize_ois(hWnd, &mInputRouter);
}

Core::~Core()
{
	if (mWin) {
		delete mWin;
	}

	if (mGameWindowEventListener) {
		delete mGameWindowEventListener;
	}
}

void Core::initialize_root(const String &pluginFilename,
		const String &configFilename,
		const String &logFilename,
		bool showConfigDialog)
{
	// Cannot initialise the root more than once
	if (!mRoot) {
		// Create the root
		mRoot = new Root(pluginFilename, configFilename, logFilename);
	}


	// Ensure the root was created
	if (!mRoot) {
		throw GameInitException("Error creating root");
	}

	// Load basic configuration
	if (showConfigDialog ? !mRoot->showConfigDialog() : !mRoot->restoreConfig()) {
		throw GameInitException("Error acquiring game configuration");
	}

	// Create a window out of the root
	mWin = mRoot->initialise(true);

	// Create a window event handler
	mGameWindowEventListener = new GameWindowEventListener(this);
	WindowEventUtilities::addWindowEventListener(mWin, mGameWindowEventListener);

	// Point the root to our frame listener
	mRoot->addFrameListener(&mFrameRouter);
}

void Core::load_ogre_resources(const String &resourceFilename)
{
	// The root must be initialized before we can load resources
	if (!mRoot) {
		throw GameInitException("Attempting to load resources before root is initialized");
	}

	ConfigFile cf;
	cf.load(resourceFilename);
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	String secName, typeName, archName;
	while (seci.hasMoreElements()) {
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

CoreStateMachine *Core::get_state_machine()
{
	return &mStateMachine;
}

InputRouter<CoreState, CoreStateMachine> *Core::get_input_router()
{
	return &mInputRouter;
}

FrameRouter<CoreState, CoreStateMachine> *Core::get_frame_router()
{
	return &mFrameRouter;
}

void Core::run(const String &shipConfigFilename)
{
	MainMenu menu(this, mWin, shipConfigFilename);
	menu.show();
	while (mStateMachine.get_current_state() != CORE_SHUTDOWN) {
		mInputManager.capture();
		Ogre::WindowEventUtilities::messagePump();
		mRoot->renderOneFrame();
	}
}
