#pragma once
#include <string>

struct telegram
{

	telegram(int sender, int receiver, int msg, float dispatchTime, void* extraInfo)
	{
		this->sender = sender;
		this->receiver = receiver;
		this->msg = msg;
		this->dispatchTime = dispatchTime;
		this->extraInfo = extraInfo;
	}
	
	//The unit that sent the telegram
	int sender;
	//The unit to receive the telegram
	int receiver;
	//The message. Follows the enum of messageTypes
	int msg;
	//If the message is immediate or delayed
	float dispatchTime;
	//Any additional information, for example a position
	void* extraInfo;
};


enum messageType
{
	msg_wantToMeet,
	msg_iCanMeet,
	msg_iCantMeet,
	msg_meetUp
};

//Returns the message based on a messageType
inline std::string msgToStr(int msg)
{
	switch (msg)
	{
	case 0:
		return "Hey, do you want to meet?";
	case 1:
		return "Sure, I meet you in a bit!";
	case 2:
		return "No sorry, I can't meet right now.";
	case 3:
		return "Let's meet with my friend";
	default:
		return "Not recognized!";
	}
}

//Needed overloaders
inline bool operator==(const telegram& t1, const telegram& t2)
{
	return (fabs(t1.dispatchTime - t2.dispatchTime) < 0.25f) &&
		(t1.sender == t2.sender) &&
		(t1.receiver == t2.receiver) &&
		(t1.msg == t2.msg);
}

inline bool operator<(const telegram& t1, const telegram& t2)
{
	if (t1 == t2)
	{
		return false;
	}
	else
	{
		return  (t1.dispatchTime < t2.dispatchTime);
	}
}
