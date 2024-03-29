/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#include "coreStateMachine.hpp"

CoreStateMachine::CoreStateMachine()
{
	mState = CORE_INVALID;
}

CoreStateMachine::~CoreStateMachine() {}


bool CoreStateMachine::request_state_change(CoreState state)
{
	if (mState == CORE_SHUTDOWN || // Cannot cancel shutdown
			state == CORE_STARTUP || // Cannot return to startup
			state == mState // Cannot change to a state we're already in
	) {
		return false;
	}

	mState = state;
	return true;
}


