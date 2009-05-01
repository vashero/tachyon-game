/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#pragma once

#include "Ogre.h"
using namespace Ogre;

#include "stateRouter.hpp"

/**
   Redirects frame events to proper handler depending on state
 */
template <class S, class SM>
class FrameRouter: public StateRouter<S, SM, FrameListener>,
public FrameListener
{
public:
	// Methods --------------------
	/** The empty constructor */
	FrameRouter(): StateRouter<S, SM, FrameListener>(){}

	/** The destructor */
	~FrameRouter()
	{
	}

	// Listener functions
	virtual bool frameStarted(const FrameEvent &evt)
	{
		FrameListener *listener = this->get_listener();

		if (!listener) {
			// There is no object registered to handle the current state
			return true;
		}

		return listener->frameStarted(evt);
	}

	virtual bool frameEnded(const FrameEvent &evt)
	{
		FrameListener *listener = this->get_listener();

		if (!listener) {
			// There is no object registered to handle the current state
			return true;
		}

		return listener->frameEnded(evt);
	}
};
