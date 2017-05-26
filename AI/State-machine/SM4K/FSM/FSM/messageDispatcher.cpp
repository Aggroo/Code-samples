#include "messageDispatcher.h"
#include "entityManager.h"
#include "Object.h"

messageDispatcher* messageDispatcher::Instance()
{
	static messageDispatcher instance;
	return &instance;
}

void messageDispatcher::sendMessage(float delay, int sender, int receiver, int msg, void* extraInfo)
{
	Object* tempSender = entityMgr->getUnitFromID(sender);
	Object* tempReceiver = entityMgr->getUnitFromID(receiver);

	if (tempReceiver == NULL)
	{
		std::cout << "\nWarning! No receiver with that ID";
		return;
	}

	telegram tlgrm(sender, receiver, msg, delay, extraInfo);

	if (delay <= 0.0f)
	{
		std::cout << "\n"<< tempSender->getName() << " : "<< msgToStr(msg);

		discharge(tempReceiver, tlgrm);
	}
	else
	{
		float currentTime = glfwGetTime();

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

		Object* tempReceiver = entityMgr->getUnitFromID(tlgrm.receiver);

		std::cout << "\n" << tempReceiver->getName() << " : " << msgToStr(tlgrm.msg);

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
