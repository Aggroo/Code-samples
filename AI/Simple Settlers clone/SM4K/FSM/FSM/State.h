#pragma once
struct telegram;
template <class unit>
class State
{
public:
	virtual void enter(unit*) = 0;

	virtual void execute(unit*) = 0;

	virtual void exit(unit*) = 0;

	virtual bool onMessage(unit*, const telegram&) = 0;
	
	virtual ~State(){}
};
