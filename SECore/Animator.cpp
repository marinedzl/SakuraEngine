#include "stdafx.h"
#include "AnimationClip.h"
#include "Animator.h"

typedef Animator::State State;
typedef Animator::Param Param;
typedef Animator::Condition Condition;
typedef Animator::Transition Transition;

struct Animator::Transition
{
	std::string name;
	State* nextState;
	std::vector<const Condition*> conditions;
	float offset;
	float length;
	virtual ~Transition()
	{
		DeleteList(conditions);
	}
};

struct Animator::State
{
	std::string name;
	bool loop;
	float length;
	const AnimationClip* clip;
	std::vector<Transition*> transitions;

	State()
		: loop(false)
	{

	}

	~State()
	{
		DeleteList(transitions);
	}
};

Animator::Animator()
	: AnimationContainer()
	, mState(nullptr)
	, mTransition(nullptr)
{

}

Animator::~Animator()
{
	DeleteMap(mStates);
}

void Animator::Update(float deltaTime)
{
	if (!mState)
		return;

	mBlendDesc.currTime += deltaTime;
	mBlendDesc.nextTime += deltaTime;

	if (mTransition)
	{
		mTransitionElapsedTime += deltaTime;
		if (mTransitionElapsedTime > mTransition->length)
		{
			// transition to next state
			mState = mTransition->nextState;
			mTransition = nullptr;

			// update blend desc
			mBlendDesc.currClip = mState->clip;
			mBlendDesc.nextClip = nullptr;
			mBlendDesc.currTime = mBlendDesc.nextTime;
		}
		else
		{
			mBlendDesc.lerp = mTransitionElapsedTime / mTransition->length;
		}
	}
	else
	{
		if (mState->loop)
		{
			if (mBlendDesc.currTime > mState->length)
			{
				mBlendDesc.currTime = fmod(mBlendDesc.currTime, mState->length);
			}
		}

		size_t tcount = mState->transitions.size();
		for (size_t t = 0; t < tcount; ++t)
		{
			const Transition * transition = mState->transitions[t];
			if (CheckCondition(transition))
			{
				// start transition
				mTransitionElapsedTime = 0;
				mTransition = transition;

				// update blend desc
				mBlendDesc.nextClip = mTransition->nextState->clip;
				mBlendDesc.lerp = mTransitionElapsedTime / mTransition->length;
				mBlendDesc.nextTime = mTransition->offset;
				break;
			}
		}
	}

	Blend(mBlendDesc);
}

bool Animator::CheckCondition(const Transition* transition)
{
	size_t count = transition->conditions.size();
	for (size_t i = 0; i < count; ++i)
	{
		const Condition* condition = transition->conditions[i];
		switch (condition->param.type)
		{
		case Param::eExitTime:
		{
			if (mBlendDesc.currTime < condition->param.value.r * mState->length)
				return false;
		}
			break;
		case Param::eBool:
		{
			Param* param = GetParam(condition->name.c_str());
			CHECK(param);
			if (!param->value.b)
				return false;
		}
			break;
		default:
			CHECK(false);
			break;
		}
	}

Exit0:
	return true;
}

Param* Animator::GetParam(const char* name)
{
	Params::iterator iter = mParams.find(name);
	return iter == mParams.end() ? nullptr : &iter->second;
}

State* Animator::GetState(const char* name)
{
	States::iterator iter = mStates.find(name);
	return iter == mStates.end() ? nullptr : iter->second;
}

bool Animator::SetParam(const char * name, const Param & src)
{
	Param* dst = GetParam(name);
	if (dst && dst->type == src.type)
	{
		*dst = src;
		return true;
	}
	return false;
}

bool Animator::CreateParam(const char * name, const Param & param)
{
	mParams.insert(std::make_pair(name, param));
	return true;
}

bool Animator::CreateState(const char * name, const AnimationClip * clip, bool loop)
{
	State* state = new State();
	state->clip = clip;
	state->length = clip->GetLength();
	state->loop = loop;
	state->name = name;
	mStates.insert(std::make_pair(name, state));

	if (mState == nullptr)
	{
		mState = state;
		mBlendDesc.currClip = mState->clip;
		mBlendDesc.currTime = 0;
	}
	return true;
}

Transition * Animator::CreateTransition(State* state, const char * name, State* nextState, float offset, float length)
{
	Transition* transition = new Transition();
	transition->name = name;
	transition->nextState = nextState;
	transition->offset = offset;
	transition->length = length;
	state->transitions.push_back(transition);
	return transition;
}

bool Animator::CreateCondition(Transition * transition, const char * name, const Param & param)
{
	Condition* condition = new Condition();
	condition->name = name;
	condition->param = param;
	transition->conditions.push_back(condition);
	return true;
}
