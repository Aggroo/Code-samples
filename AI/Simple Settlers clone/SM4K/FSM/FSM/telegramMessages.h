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
	msg_UpgradeExplorer,
	msg_UpgradeBuilder,
	msg_UpgradeCoalBurner,
	msg_SearchWood,
	msg_BuildCharcoalBurner,
	msg_HarvestWood,
	msg_ProduceCoal,
	msg_BringHomeHarvest,
	msg_BuildingDone,
	msg_NeedBuilder
};

//Returns the message based on a messageType
inline std::string msgToStr(int msg)
{
	switch (msg)
	{
	case 0:
		return "UpgradeExplorer";
	case 1:
		return "UpgradeBuilder";
	case 2:
		return "UpgradeCoalBurner";
	case 3:
		return "SearchWood";
	case 4:
		return "BuildCharcoalBurner";
	case 5:
		return "HarvestWood";
	case 6:
		return "ProduceCoal";
	case 7:
		return "BringHomeHarvest";
	case 8:
		return "BuildingDone";
	case 9:
		return "NeedBuilder";
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
