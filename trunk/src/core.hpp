/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#pragma once

#include <Ogre.h>
using namespace Ogre;

#include "coreStateMachine.hpp"
#include "coreInputManager.hpp"
#include "inputRouter.hpp"
#include "frameRouter.hpp"

class GameWindowEventListener;

/**
   Handles ogre initialization and cleanup.
   Also holds all high-level objects essential to game operation
 */
class Core
{
public:
	// Methods --------------------

	/** The empty constructor */
	Core(const String &pluginFilename, const String &configFilename,
			const String &logFilename, const String &resourceFilename,
			bool showConfigDialog);

	/** The destructor */
	~Core();

	/** Initialize the root object
      @param pluginFilename The path of the plugins config file
      @param configFilename The path of the ogre config file
      @param logFilename The log file ogre will create
      @param showConfigDialog Whether or not to show the ogre config dialog
	 */
	void initialize_root(const String &pluginFilename, const String &configFilename,
			const String &logFilename, bool showConfigDialog);

	/** Load ogre resources
      @param resourceFilename The resource config file that points to all the
      resources we want to load
	 */
	void load_ogre_resources(const String &resourceFilename);

	/** Get the high-level state machine
      @return The state machine
	 */
	CoreStateMachine *get_state_machine();

	/** Get the high-level input router
      @return The input router
	 */
	InputRouter<CoreState, CoreStateMachine> *get_input_router();

	/** Get the high-level frame router
      @return The frame router
	 */
	FrameRouter<CoreState, CoreStateMachine> *get_frame_router();

	/** Run the game */
	void run(const String &shipConfigFilename);

protected:
	// Variables --------------------

	/** The state we are currently in */
	CoreStateMachine mStateMachine;

	/** The input router */
	InputRouter<CoreState, CoreStateMachine> mInputRouter;

	/** The frame router */
	FrameRouter<CoreState, CoreStateMachine> mFrameRouter;

	/** Our root object */
	Root *mRoot;

	/** The window we will render our game in */
	RenderWindow *mWin;

	/** The game input manager */
	CoreInputManager mInputManager;

	/** The window event listener */
	GameWindowEventListener *mGameWindowEventListener;
};


/** The window event listener, which will handle window events (resizing, focus
    changing, etc
 */
class GameWindowEventListener:public WindowEventListener
{
public:
	Core *mCore;
	GameWindowEventListener(Core *core)
	{
		mCore = core;
	}

	// Adjust the mouse clipping area
	virtual void windowResized(RenderWindow *rw) {
	}

	// Unattach OIS before window shutdown
	virtual void windowClosed(RenderWindow *rw)
	{
		mCore->get_state_machine()->request_state_change(CORE_SHUTDOWN);
	}
};
