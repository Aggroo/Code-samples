#include "messageDispatcher.h"
#include "UnitHandler.h"
#include "Object.h"

messageDispatcher* messageDispatcher::Instance()
{
	static messageDispatcher instance;
	return &instance;
}

void messageDispatcher::sendMessage(float delay, int sender, int receiver, int msg, void* extraInfo)
{
	//Object* tempSender = UnitHandler::Instance()->GetUnit(sender);
	Object* tempReceiver = UnitHandler::Instance()->GetUnit(receiver);

	if (tempReceiver == NULL)
	{
		std::cout << "\nWarning! No receiver with that ID";
		return;
	}

	telegram tlgrm(sender, receiver, msg, delay, extraInfo);
	std::string str = msgToStr(msg);
	if (delay <= 0.0f)
	{
		//if (sender == 0)
		//{
		//	
		//	printf("<HiveMind> : %s \n", str.c_str());
		//}
		//else
		//{
		//	printf("Unit %i : %s \n", sender, str.c_str());
		//}
		
		//std::cout << "\n" << sender << " : " << msgToStr(msg);

		discharge(tempReceiver, tlgrm);
	}
	else
	{
		float currentTime = glfwGetTime();

		//if (sender == 0)
		//{
		//	printf("<HiveMind> : %s \n", str.c_str());
		//}
		//else
		//{
		//	printf("Unit %i : %s \n", sender, str.c_str());
		//}
		tlgrm.dispatchTime = currentTime + delay;

		prioQueue.insert(tlgrm);

	}
}

void messageDispatcher::sendDelayedMessage()
{
	float currentTime = glfwGetTime();

	while (!prioQueue.empty() && (prioQueue.begin()->dispatchTime < currentTime) && (prioQueue.begin()->dispatchTime > 0))
	{
		const telegram& tlgrm = *prioQueue.begin();

		Object* tempReceiver = UnitHandler::Instance()->GetUnit(tlgrm.receiver);

		//std::cout << "\n" << tempReceiver- << " : " << msgToStr(tlgrm.msg);

		discharge(tempReceiver, tlgrm);

		prioQueue.erase(prioQueue.begin());
	}
}

void messageDispatcher::discharge(Object* receiver, const telegram& msg)
{
	if (!receiver->handleMessage(msg))
	{
		printf("Message not handled");
	}
}
