/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#pragma once

#include "inputRouter.hpp"

#include "CEGUISystem.h"
#include "CEGUIWindow.h"

#include "Ogre.h"
using namespace Ogre;

#include <stack>
using namespace std;

/** States that the menu can be in */
typedef enum {
	MAIN_MENU_ROOT, /**< The top of the main menu */
	MAIN_MENU_OPTIONS, /**< The top-level options menu */
	MAIN_MENU_OPTIONS_MORE,
	MAIN_MENU_MODEL
} GameMenuState;

/** Filenames of the sheet corresponding to each state */
const String GAME_MENU_LAYOUT_FILENAMES[] = {
		"gui_root.layout", "gui_options.layout", "gui_options_more.layout", "gui_model.layout"
};

const String GAME_MENU_ROOT_NAME = "rootWindow";

/**
   The base class for each menu.  Handles initializing the gui
   for the relevant scene, and allows creation of various preset
   windows
 */
class GameMenu: public InputListener
{
public:
	GameMenu(RenderWindow *win, SceneManager *sceneMgr);

	~GameMenu();

	/** Push a menu state onto the top of the
      state stack, and switch to that state
      @param state The state to switch to
      @return The window created from pushing the new menu state
	 */
	CEGUI::Window *push_state(GameMenuState state);

	/** Remove the currently active state and
      activate the state above it
	 */
	CEGUI::Window *pop_state();

	/** Get the top-level window */
	CEGUI::Window *get_root();

	/** Get the prefix of the current state */
	String get_prefix_name();

	// Static functions -----------------
	/** Get the prefix associated with the passed state
      @param state The relevant state
	 */
	static String get_prefix_name(GameMenuState state);

	// Listener functions
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

protected:
	// Variables --------------------
	SceneManager *mSceneMgr;
	RenderWindow *mWin;
	stack<GameMenuState> mStateStack;
	CEGUI::System *mSystem;

	String mResourceGroup;
	String mSchemeFilename;
	String mSchemeName;
	String mMouseName;
	String mFontFilename;
	String mFontName;

	String mRootLayoutFilename;

	// Methods --------------------
	/** Set the provided menu state
      @param state The state to switch to
      @return The window created from setting the new state
	 */
	CEGUI::Window *set_state(GameMenuState state);

	/** Unset the provided menu state
      @param state The state to unset
      @param visible If this is false, the relevant menu is made invisible
	 */
	void unset_state(GameMenuState state, bool visible);

	/** Initialize CEGUI resources */
	bool initialize_gui();
};
