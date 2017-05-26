#pragma once
#include "telegramMessages.h"
#include <set>

class Object;

#define dispatcher messageDispatcher::Instance()

class messageDispatcher
{
public:
	//Singleton instancer
	static messageDispatcher* Instance();

	//Used to send messages to other units
	void sendMessage(float delay, int sender, int receiver, int msg, void* extraInfo);

	//Used in the update if there is any messages in the prioQueue
	void sendDelayedMessage();

private:
	//Stores delayed messages
	std::set<telegram> prioQueue;

	//Checks if the message was handled
	void discharge(Object* receiver, const telegram& msg);

	//Constructors
	messageDispatcher(){}
	messageDispatcher(const messageDispatcher&) = delete;
	messageDispatcher& operator=(const messageDispatcher&) = delete;
};

