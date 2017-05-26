#pragma once
#include "State.h"

class Unit;

//Idle state. Only used at the start of the program
class idle: public State<Unit>
{
public:
	static idle* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	idle(){}

	idle(const idle&) = delete;
	idle& operator=(const idle&) = delete;
};

//goToWork state. Used to make the unit walk to work, get money and loose fatigue
class goToWork: public State<Unit>
{
public:
	static goToWork* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	goToWork(){}

	goToWork(const goToWork&) = delete;
	goToWork& operator=(const goToWork&) = delete;
};

//enterShopToBuyClothes state. Used to make the unit walk to a shop and buy a new shirt. (Changes their color)
class enterShopToBuyClothes: public State<Unit>
{
public:
	static enterShopToBuyClothes* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	enterShopToBuyClothes(){}

	enterShopToBuyClothes(const enterShopToBuyClothes&) = delete;
	enterShopToBuyClothes& operator=(const enterShopToBuyClothes&) = delete;
};

//goToTheRestaurant state. Used to make the unit walk to a restaurant to get less hunger and thirst.
class goToTheRestaurant: public State<Unit>
{
public:
	static goToTheRestaurant* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	goToTheRestaurant(){}

	goToTheRestaurant(const goToTheRestaurant&) = delete;
	goToTheRestaurant& operator=(const goToTheRestaurant&) = delete;
};

//goHomeAndSleep state. Used to make a unit walk home to sleep
class goHomeAndSleep: public State<Unit>
{
public:
	static goHomeAndSleep* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	goHomeAndSleep(){}

	goHomeAndSleep(const goHomeAndSleep&) = delete;
	goHomeAndSleep& operator=(const goHomeAndSleep&) = delete;
};

//goToTheBar state. Used to make the unit walk to a pub. Makes them less thirsty and is used for a place to meetup and socialize
class goToTheBar: public State<Unit>
{
public:
	static goToTheBar* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	goToTheBar(){}

	goToTheBar(const goToTheBar&) = delete;
	goToTheBar& operator=(const goToTheBar&) = delete;
};

//death state. Used to make the unit walk to a graveyard and die
class death : public State < Unit >
{
public:
	static death* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	death(){}

	death(const death&) = delete;
	death& operator=(const death&) = delete;
};

//globalStates state. Used to handle all needs that happens in all states. For example if a unit get hungry or thirsty.
class globalStates : public State<Unit>
{
public:
	static globalStates* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	globalStates(){}

	globalStates(const globalStates&) = delete;
	globalStates& operator=(const globalStates&) = delete;
};

