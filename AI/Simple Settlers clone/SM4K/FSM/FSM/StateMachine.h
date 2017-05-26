#pragma once
#include "State.h"

template <class unit>
class StateMachine
{
public:
	StateMachine(unit* owner) :owner(owner), currentState(NULL), previousState(NULL), globalState(NULL) {}

	//Get CurrentState of the agent
	State<unit>* getCurrentState() const { return currentState; }

	//Sets the currentState of the agent
	void setCurrentState(State<unit>* val) { currentState = val; }

	//Get the previousState of the agent
	State<unit>* getPreviousState() const { return previousState; }

	//Set the previousState of the agent
	void setPreviousState(State<unit>* val) { previousState = val; }

	//Get the globalState
	State<unit>* getGlobalState() const { return globalState; }

	//Sets the globalState
	void setGlobalState(State<unit>* val) { globalState = val; }

	//Updates the FSM
	void update(float deltaTime)
	{
		if (globalState)
			globalState->execute(owner);
		
		if (currentState)
			currentState->execute(owner);
	}

	//Handles the messages. Checks from where the unit checked the message
	bool handleMessage(const telegram& msg)
	{
		if (currentState && currentState->onMessage(owner, msg))
		{
			return true;
		}
		
		if (globalState && globalState->onMessage(owner, msg))
		{
			return true;
		}

		return false;

	}

	//Changes currentState to the newState
	void changeState(State<unit>* newState)
	{
		assert(newState && "<StateMachine::ChangeState>: trying to change to a null state");

		previousState = currentState;
		currentState->exit(owner);

		currentState = newState;
		currentState->enter(owner);
	}
	
	//Changes state to the previousState of the agent
	void revertToPreviousState()
	{
		changeState(previousState);
	}

	bool isInState(const State<unit>& st) const;

private:
	//Pointer to the agent who owns this instance
	unit* owner;

	//The current state of the agent
	State<unit>* currentState;

	//The previous state the agent was in
	State<unit>* previousState;
	
	//Called every time the FSM is updated. Used for hunger, thirst and fatigue
	State<unit>* globalState;

};


