#pragma once

/** The base state machine class */
template <class S>
class GameStateMachine
{
public:
	/** The empty constructor */
	GameStateMachine(){}

	/** The destructor */
	~GameStateMachine(){}

	/** Request that we move to a different state
	 *  @param state The state that we would like to move to
	 *  @return True if we have successfully changed state; false otherwise
	 */
	virtual bool request_state_change(S state) = 0;

	/** Get the current state that we are in
	 * 	@return Our current state
	 */
	S get_current_state()
	{
		return mState;
	}
protected:
	S mState;
};
