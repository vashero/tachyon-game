/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#pragma once

#include "arenaView.hpp"
#include "gameMenu.hpp"

#include "CEGUISystem.h"

#include "core.hpp"
#include "CEGUIFreeFunctionSlot.h"

#include <stack>
using namespace std;

const Real MAIN_MENU_DEF_ROTATE_SPEED = 20;

/**
   Displays the main menu, and handles all user input.
   Passes all data-level requests to the appropriate core object
 */
class MainMenu: public InputListener, public FrameListener
{
public:
	// Methods --------------------
	MainMenu(Core *core, RenderWindow *win, const String &shipConfigFilename);

	/** The destructor */
	~MainMenu();

	/** Quit the game */
	void quit();

	/** Show the top-level gui */
	void show();

	// Button event handlers
	bool New_Game_OnClick(const CEGUI::EventArgs &args);
	bool Options_OnClick(const CEGUI::EventArgs &args);
	bool Model_OnClick(const CEGUI::EventArgs &args);
	bool Quit_OnClick(const CEGUI::EventArgs &args);
	bool Back_OnClick(const CEGUI::EventArgs &args);
	bool More_OnClick(const CEGUI::EventArgs &args);
	bool Cruiser_OnClick(const CEGUI::EventArgs &args);
	bool Superiority_OnClick(const CEGUI::EventArgs &args);
	bool Test_OnClick(const CEGUI::EventArgs &args);

	// Input listener functions
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	// Frame listener functions
	virtual bool frameStarted(const FrameEvent &evt);
protected:
	// Variables --------------------
	Core *mCore;
	RenderWindow *mWin;
	SceneManager *mSceneMgr;
	ArenaView *mArenaView;
	GameMenu *mGameMenu;
	Real mRotateSpeed;

	/** TODO: Temp */
	bool mCameraControl;

	/** Set callbacks for relevant child buttons of the passed window
      @param window The window holding all buttons whose 'clicked' events
      are to be subscribed to
	 */
	void set_callbacks(CEGUI::Window *window);

	/** Create the scene to render the GUI */
	bool create_scene(const String &shipConfigFilename);

	/** Switch the model being displayed */
	void switch_model(const ShipViewType &newModel);

	/** Move the menu back one.  If currently at the top menu, quit */
	void menu_back();
};
