#pragma once

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "Ogre.h"
using namespace Ogre;

#include "stateRouter.hpp"

/** The class that all listeners must inherit */
class InputListener: public OIS::KeyListener, public OIS::MouseListener{};

/**
   Redirects input to proper input manager depending on state
 */
template <class S, class SM>
class InputRouter: public StateRouter<S, SM, InputListener>,
public InputListener
{
public:
	// Methods --------------------
	/** The empty constructor */
	InputRouter(): StateRouter<S, SM, InputListener>(){}

	/** The destructor */
	~InputRouter()
	{
	}

	// Listener functions
	virtual bool mouseMoved(const OIS::MouseEvent &arg)
	{
		InputListener *listener = this->get_listener();

		if (!listener) {
			// There is no object registered to handle the current state
			return true;
		}

		return listener->mouseMoved(arg);
	}

	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		InputListener *listener = this->get_listener();

		if (!listener) {
			// There is no object registered to handle the current state
			return true;
		}

		return listener->mousePressed(arg, id);
	}

	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		InputListener *listener = this->get_listener();

		if (!listener) {
			// There is no object registered to handle the current state
			return true;
		}

		return listener->mouseReleased(arg, id);
	}

	virtual bool keyPressed(const OIS::KeyEvent &arg)
	{
		InputListener *listener = this->get_listener();

		if (!listener) {
			// There is no object registered to handle the current state
			return true;
		}

		return listener->keyPressed(arg);
	}

	virtual bool keyReleased(const OIS::KeyEvent &arg)
	{
		InputListener *listener = this->get_listener();

		if (!listener) {
			// There is no object registered to handle the current state
			return true;
		}

		return listener->keyReleased(arg);
	}
};
