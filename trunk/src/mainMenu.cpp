#include "mainMenu.hpp"

#include "elements/CEGUIPushButton.h"

MainMenu::MainMenu(Core *core, RenderWindow *win, const String &shipConfigFilename)
{
	mCameraControl = false;
	mCore = core;
	mWin = win;

	mRotateSpeed = MAIN_MENU_DEF_ROTATE_SPEED;

	mSceneMgr = NULL;
	mArenaView = NULL;
	mGameMenu = NULL;

	// Create the main menu scene
	this->create_scene(shipConfigFilename);

	// Create the actual menu object
	mGameMenu = new GameMenu(mWin, mSceneMgr);

	// subscribe to be the input listener for main menu events
	mCore->get_state_machine()->request_state_change(CORE_MAIN_MENU);
	mCore->get_input_router()->register_listener(this, CORE_MAIN_MENU);
	mCore->get_frame_router()->register_listener(this, CORE_MAIN_MENU);
}

MainMenu::~MainMenu()
{
	if (mArenaView) {
		delete mArenaView;
	}

	if (mGameMenu) {
		delete mGameMenu;
	}
}

void MainMenu::quit()
{
	mCore->get_state_machine()->request_state_change(CORE_SHUTDOWN);
}

void MainMenu::show()
{
	this->set_callbacks(mGameMenu->push_state(MAIN_MENU_ROOT));
}

bool MainMenu::New_Game_OnClick(const CEGUI::EventArgs &args)
{
	return true;
}

bool MainMenu::Options_OnClick(const CEGUI::EventArgs &args)
{
	this->set_callbacks(mGameMenu->push_state(MAIN_MENU_OPTIONS));
	return true;
}

bool MainMenu::Model_OnClick(const CEGUI::EventArgs &args)
{
	this->set_callbacks(mGameMenu->push_state(MAIN_MENU_MODEL));
	return true;
}

bool MainMenu::Quit_OnClick(const CEGUI::EventArgs &args)
{
	this->quit();
	return true;
}

bool MainMenu::Back_OnClick(const CEGUI::EventArgs &args)
{
	this->menu_back();
	return true;
}

bool MainMenu::More_OnClick(const CEGUI::EventArgs &args)
{
	this->set_callbacks(mGameMenu->push_state(MAIN_MENU_OPTIONS_MORE));
	return true;
}

bool MainMenu::Cruiser_OnClick(const CEGUI::EventArgs &args)
{
	this->switch_model(SHIP_VIEW_CRUISER);
	return true;
}

bool MainMenu::Superiority_OnClick(const CEGUI::EventArgs &args)
{
	this->switch_model(SHIP_VIEW_SUPERIORITY);
	return true;
}

bool MainMenu::Test_OnClick(const CEGUI::EventArgs &args)
{
	this->switch_model(SHIP_VIEW_NONE);
	return true;
}

bool MainMenu::mouseMoved(const OIS::MouseEvent &evt)
{
	if (mCameraControl) {
		mArenaView->rotate_camera(Radian((float)evt.state.X.rel/10),
				Radian((float)evt.state.Y.rel/10));
	}
	if (!mGameMenu) {
		return true;
	}
	return mGameMenu->mouseMoved(evt);
}

bool MainMenu::mousePressed(const OIS::MouseEvent &evt,
		OIS::MouseButtonID mouseButtonID)
{
	if (!mGameMenu || !mGameMenu->mousePressed(evt, mouseButtonID)) {
		// If a mouse button has been pressed outside of the gui, take
		// control of the camera
		mCameraControl = true;
		CEGUI::MouseCursor::getSingleton().hide();
	}
	return true;
}

bool MainMenu::mouseReleased(const OIS::MouseEvent &evt,
		OIS::MouseButtonID mouseButtonID)
{
	mCameraControl = false;
	CEGUI::MouseCursor::getSingleton().show();
	if (!mGameMenu) {
		return true;
	}
	return mGameMenu->mouseReleased(evt, mouseButtonID);
}

bool MainMenu::keyPressed(const OIS::KeyEvent &evt)
{
	if (evt.key == OIS::KC_ESCAPE) {
		this->menu_back();
	}
	if (!mGameMenu) {
		return true;
	}
	return mGameMenu->keyPressed(evt);
}

bool MainMenu::keyReleased(const OIS::KeyEvent &evt)
{
	if (!mGameMenu) {
		return true;
	}
	return mGameMenu->keyReleased(evt);
}

bool MainMenu::frameStarted(const FrameEvent &evt)
{
	if (!mCameraControl) {
		mArenaView->rotate_camera(Degree(mRotateSpeed * evt.timeSinceLastFrame),
				Degree(0));
	}
	return true;
}

void MainMenu::set_callbacks(CEGUI::Window *window)
{
	String prefixName = mGameMenu->get_prefix_name();
	for (unsigned int i = 0; i < window->getChildCount(); i++) {
		CEGUI::Window *childWin = window->getChildAtIdx(i);
		CEGUI::String winName = childWin->getName();

		// The prefix is not needed for assigning callbacks
		winName = winName.substr(prefixName.length());
		if (winName == "cmd_New_Game") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::New_Game_OnClick, this));
		} else if (winName == "cmd_Options") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::Options_OnClick, this));
		} else if (winName == "cmd_Model") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::Model_OnClick, this));
		} else if (winName == "cmd_Quit") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::Quit_OnClick, this));
		} else if (winName == "cmd_Back") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::Back_OnClick, this));
		} else if (winName == "cmd_More") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::More_OnClick, this));
		} else if (winName == "cmd_Cruiser") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::Cruiser_OnClick, this));
		} else if (winName == "cmd_Superiority") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::Superiority_OnClick, this));
		} else if (winName == "cmd_Test") {
			childWin->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&MainMenu::Test_OnClick, this));
		}
	}
}

bool MainMenu::create_scene(const String &shipConfigFilename)
{
	if (!mWin) {
		// Need a window to render in
		return false;
	}

	if (mSceneMgr) {
		// If we already have created a scene manager, just empty it
		mSceneMgr->clearScene();
	} else {
		mSceneMgr = Root::getSingletonPtr()->createSceneManager(ST_GENERIC, "GUIManager");
	}

	// Create the view that will be in the background
	mArenaView = new ArenaView(mWin, shipConfigFilename, "GUIManager");
	// Rotate the camera so we're not directly above the model
	mArenaView->rotate_camera(Radian(0), Radian(Degree(45)));

	// Add a ship to the arena view
	this->switch_model(SHIP_VIEW_TEST);

	return true;
}

void MainMenu::switch_model(const ShipViewType &newModel)
{
	mArenaView->clear_ships();

	if (newModel != SHIP_VIEW_NONE) {
		mArenaView->add_ship(newModel, Vector3::ZERO, Radian(0));
	}
}

void MainMenu::menu_back()
{
	if (mGameMenu) {
		if (!mGameMenu->pop_state()) {
			this->quit();
		}
	}
}

