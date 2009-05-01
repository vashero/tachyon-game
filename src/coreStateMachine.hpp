/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#pragma once

#include "gameStateMachine.hpp"

/** The high-level game state. */
typedef enum {
	CORE_INVALID, /**< The current state is invalid */
	CORE_STARTUP, /**< The system is starting up */
	CORE_MAIN_MENU, /**< We are at the main menu */
	//CORE_BATTLE, /**< A battle is in progress */
	CORE_SHUTDOWN /**< The system is shutting down */
} CoreState;

/**
   The state machine controlling the high-level game state.
 */
class CoreStateMachine: public GameStateMachine<CoreState>
{
public:
	// Methods --------------------
	/** The empty constructor */
	CoreStateMachine();

	/** The destructor */
	~CoreStateMachine();

	// Override virtual methods
	bool request_state_change(CoreState state);
};
