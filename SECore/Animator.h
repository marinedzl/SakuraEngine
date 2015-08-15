#pragma once
#include "AnimationContainer.h"

class Animator : public AnimationContainer
{
public:
	virtual ~Animator();
	virtual bool SetBool(const char* name, bool value) { Param param; param.type = Param::eBool; param.value.b = value; return SetParam(name, param); }
public:
	struct Param
	{
		enum Type
		{
			eExitTime,
			eBool,
		} type;
		union Value
		{
			float r;
			int n;
			bool b;
		} value;
	};
	struct Condition
	{
		std::string name;
		Param param;
	};
	struct Transition;
	struct State;
public:
	Animator();
	void Update(float deltaTime);
public:
	bool CreateParam(const char* name, const Param& param);
	bool CreateState(const char* name, const AnimationClip* clip, bool loop);
	Transition* CreateTransition(State* state, const char* name, State* nextState, float offset, float length);
	bool CreateCondition(Transition* transition, const char* name, const Param& param);
	State* GetState(const char* name);
	bool SetParam(const char* name, const Param& param);
	Param* GetParam(const char* name);
private:
	typedef std::map<std::string, State*> States;
	typedef std::map<std::string, Param> Params;
private:
	bool CheckCondition(const Transition* transition);
private:
	BlendDesc mBlendDesc;
	States mStates;
	const State* mState;
	float mTransitionElapsedTime;
	const Transition* mTransition;
	Params mParams;
};
