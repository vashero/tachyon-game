#include "gameMenu.hpp"
#include "gameException.hpp"

#include "OgreCEGUIRenderer.h"
#include "CEGUISystem.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowManager.h"
#include "CEGUISchemeManager.h"
#include "CEGUIFontManager.h"
#include "elements/CEGUIPushButton.h"

GameMenu::GameMenu(RenderWindow *win, SceneManager *sceneMgr)
{
	// Initialize variables that are currently constant, but may be
	// changeable in future versions
	mResourceGroup = "GUI";
	mSchemeFilename = "TaharezLookSkin.scheme";
	mSchemeName = "TaharezLook";
	mMouseName = "MouseArrow";
	mFontFilename = "";
	mFontName = "";

	mWin = win;
	mSceneMgr = sceneMgr;

	this->initialize_gui();
}

GameMenu::~GameMenu()
{
}

CEGUI::Window *GameMenu::push_state(GameMenuState state)
{
	// Make the current state inactive, and invisible
	if (!mStateStack.empty()) {
		this->unset_state(mStateStack.top(), false);
	}

	mStateStack.push(state);
	return this->set_state(state);
}

CEGUI::Window *GameMenu::pop_state()
{
	// Make sure we actually have state
	if (mStateStack.empty()) {
		return NULL;
	}

	// Make the current window invisible and inactive
	this->unset_state(mStateStack.top(), false);
	mStateStack.pop();
	if (mStateStack.empty()) {
		// If we were at the top-level screen, quit
		return NULL;
	} else {
		// If there was a state above this one, go to that state
		return this->set_state(mStateStack.top());
	}
}

CEGUI::Window *GameMenu::get_root()
{
	if (mStateStack.empty()) {
		return NULL;
	}

	return CEGUI::WindowManager::getSingleton().getWindow
	(GAME_MENU_LAYOUT_FILENAMES[mStateStack.top()]);
}

CEGUI::Window *GameMenu::set_state(GameMenuState state)
{
	string layoutName = GAME_MENU_LAYOUT_FILENAMES[state];
	CEGUI::Window *layout;
	// Check if the window has already been loaded.  Note that this assumes
	// that the root window is named the same as the layout file it is defined in
	if (CEGUI::WindowManager::getSingleton().isWindowPresent(layoutName)) {
		// The window has already been loaded, so just get that window
		layout = CEGUI::WindowManager::getSingleton().getWindow(layoutName);

		// Unhide the window
		layout->setEnabled(true);
		layout->setVisible(true);
	} else {
		// The window doesn't exist, so create it
		string prefixName = GameMenu::get_prefix_name(state);
		layout = CEGUI::WindowManager::getSingleton().loadWindowLayout
		(layoutName, prefixName, mResourceGroup);

		// Rename the root window so that it is the same name as the layout
		// file it is defined in
		layout->rename(layoutName);

		// Create the window
		CEGUI::Window *rootWindow = CEGUI::WindowManager::getSingleton().getWindow
		(GAME_MENU_ROOT_NAME);
		rootWindow->addChildWindow(layout);
	}
	return layout;
}

void GameMenu::unset_state(GameMenuState state, bool visible)
{
	// Make sure the layout is present
	string layoutName = GAME_MENU_LAYOUT_FILENAMES[state];
	if (CEGUI::WindowManager::getSingleton().isWindowPresent(layoutName)) {
		CEGUI::Window *layout = CEGUI::WindowManager::getSingleton().getWindow(layoutName);
		layout->setEnabled(false);
		if (!visible) {
			layout->setVisible(false);
		}
	}
}

bool GameMenu::initialize_gui()
{
	if (!mSceneMgr || !mWin) {
		// Need a scene manager and a window
		throw GameInitException("GameMenu attempted to initialize gui without scene manager or render window");
	}

	CEGUI::OgreCEGUIRenderer *pGUIRenderer = new CEGUI::OgreCEGUIRenderer
	(mWin, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);

	// Can only have one system at a time, so if the system
	// exists delete it
	mSystem = CEGUI::System::getSingletonPtr();
	if (mSystem) {
		delete mSystem;
	}

	// Create the new system
	mSystem = new CEGUI::System(pGUIRenderer);

	// Set logging to informative
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	// Load necessary files
	CEGUI::SchemeManager::getSingleton().loadScheme(mSchemeFilename, mResourceGroup);

	// Set the default mouse cursor
	mSystem->setDefaultMouseCursor(mSchemeName, mMouseName);

	// Set the gui font
	if (mFontFilename != "") {
		CEGUI::FontManager::getSingleton().createFont(mFontFilename, mResourceGroup);
	}
	if (mFontName != "") {
		mSystem->setDefaultFont(mFontName);
	}

	// Create the root window
	CEGUI::Window *root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", GAME_MENU_ROOT_NAME);
	CEGUI::System::getSingleton().setGUISheet(root);

	return true;
}

bool GameMenu::mouseMoved(const OIS::MouseEvent &evt)
{
	mSystem->injectMouseWheelChange(evt.state.Z.rel);
	return mSystem->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

bool GameMenu::mousePressed(const OIS::MouseEvent &evt,
		OIS::MouseButtonID mouseButtonID)
{
	CEGUI::MouseButton button;
	switch (mouseButtonID) {
	case OIS::MB_Left:
		button = CEGUI::LeftButton;
		break;
	case OIS::MB_Middle:
		button = CEGUI::MiddleButton;
		break;
	case OIS::MB_Right:
		button = CEGUI::RightButton;
		break;
	default:
		button = CEGUI::NoButton;
		break;
	}
	return mSystem->injectMouseButtonDown(button);
}

bool GameMenu::mouseReleased(const OIS::MouseEvent &evt,
		OIS::MouseButtonID mouseButtonID)
{
	CEGUI::MouseButton button;
	switch (mouseButtonID) {
	case OIS::MB_Left:
		button = CEGUI::LeftButton;
		break;
	case OIS::MB_Middle:
		button = CEGUI::MiddleButton;
		break;
	case OIS::MB_Right:
		button = CEGUI::RightButton;
		break;
	default:
		button = CEGUI::NoButton;
		break;
	}

	return mSystem->injectMouseButtonUp(button);
}

bool GameMenu::keyPressed(const OIS::KeyEvent &evt)
{
	mSystem->injectKeyDown(evt.key);
	return mSystem->injectChar(evt.text);
}

bool GameMenu::keyReleased(const OIS::KeyEvent &evt)
{
	return mSystem->injectKeyUp(evt.key);
}

String GameMenu::get_prefix_name()
{
	if (mStateStack.empty()) {
		throw GameStateException("Need state to get prefix name");
	}
	return GameMenu::get_prefix_name(mStateStack.top());
}

String GameMenu::get_prefix_name(GameMenuState state)
{
	return GAME_MENU_LAYOUT_FILENAMES[state];
}
