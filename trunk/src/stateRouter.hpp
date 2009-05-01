/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#pragma once

#include <vector>
using namespace std;

/**
   Defines a class that can pass various events to registered listeners
   based on some state
   S is the state class
   SM is the state machine class
   L is the listener class
 */
template <class S, class SM, class L>
class StateRouter
{
public:
	/** The default constructor */
	StateRouter()
	{
		mStateMachine = NULL;
	}

	/** The destructor */
	~StateRouter(){}

	/** Set the state machine
      @param sm The state machine that will be used to redirect events
	 */
	void set_state_machine(SM *sm)
	{
		mStateMachine = sm;
	}


	/** Register to receive events when the state machine is in the provided
      state
      @param l The listener to receive events
      @param s The listener will only receive events when the state machine
      is in this state
      @return Whether or not the listener was successfully registered
	 */
	bool register_listener(L *l, S s)
	{
		// Find out if we already have a listener for the passed state
		if (this->get_state_idx(s) != -1) {
			// We already have a listener, so fail
			return false;
		}

		// Add to our list of listeners
		mListeners.push_back(l);
		mListenerStates.push_back(s);

		return true;
	}


	/** Stop sending frame information to the listener registered on the
      passed state
      @param s The state to remove a listener from
	 */
	void unregister_listener(S s)
	{
		// Find the relevant state
		int idx = this->get_state_idx(s);

		// Remove the relevant listener and state
		if (idx != -1) {
			mListeners.erase(mListeners.begin() + idx);
			mListenerStates.erase(mListenerStates.begin() + idx);
		}
	}

protected:
	// Variables --------------------
	/** The state machine */
	SM *mStateMachine;

	/** Registered listeners */
	vector<L*> mListeners;

	/** The corresponding state of each listener */
	vector<S> mListenerStates;

	// Methods --------------------
	/** Get the listener for the passed state
      @param s The relevant state
      @return The listener.  NULL if there is none
	 */
	L *get_listener(S s)
	{
		int idx = this->get_state_idx(s);
		if (idx == -1) {
			return NULL;
		} else {
			return mListeners[idx];
		}
	}

	L *get_listener()
	{
		if (!mStateMachine) {
			return NULL;
		}
		return this->get_listener(mStateMachine->get_current_state());
	}


	/** Get the index into the state that's been registered
      @param s The relevant state
      @return An index into our mListeners and mListenerStates vectors.  -1
      if there is no listener for the passed state
	 */
	int get_state_idx(S s)
	{
		// Find the relevant state
		typename vector<S>::iterator foundState;
		foundState = find(mListenerStates.begin(), mListenerStates.end(), s);
		if (foundState == mListenerStates.end()) {
			return -1;
		} else {
			return foundState - mListenerStates.begin();
		}
	}

};

