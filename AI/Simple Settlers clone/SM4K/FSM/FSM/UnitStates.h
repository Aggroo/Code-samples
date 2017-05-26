#pragma once
#include "State.h"

class Unit;

//Idle state. Used when workers have no work to do
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

//GetWood state. Searches for trees and cuts them down
class GetWood: public State<Unit>
{
public:
	static GetWood* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	GetWood(){}

	GetWood(const GetWood&) = delete;
	GetWood& operator=(const GetWood&) = delete;
};

//LeaveWood state. Leaves cut down wood
class LeaveWood : public State < Unit >
{
public:
	static LeaveWood* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	LeaveWood(){}

	LeaveWood(const LeaveWood&) = delete;
	LeaveWood& operator=(const LeaveWood&) = delete;
};

//Upgrade state. Used to make the unit upgrade to another profession
class Upgrade : public State < Unit >
{
public:
	static Upgrade* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	Upgrade(){}

	Upgrade(const Upgrade&) = delete;
	Upgrade& operator=(const Upgrade&) = delete;
};

//ProduceCoal state. Used to make a CoalBurner unit make coal
class ProduceCoal : public State < Unit >
{
public:
	static ProduceCoal* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	ProduceCoal(){}

	ProduceCoal(const ProduceCoal&) = delete;
	ProduceCoal& operator=(const ProduceCoal&) = delete;
};

//BuildBuilding state. Used to make a builder build a building
class BuildBuilding : public State < Unit >
{
public:
	static BuildBuilding* Instance();

	virtual void enter(Unit* unit);
	virtual void execute(Unit* unit);
	virtual void exit(Unit* unit);
	virtual bool onMessage(Unit* unit, const telegram& msg);

private:
	BuildBuilding(){}

	BuildBuilding(const BuildBuilding&) = delete;
	BuildBuilding& operator=(const BuildBuilding&) = delete;
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

