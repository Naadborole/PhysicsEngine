#pragma once

#include "PrimeEngine/Events/Component.h"
#include "CharacterControl/Events/Events.h"


namespace CharacterControl {

	namespace Components {

		struct PoolBall : public PE::Components::Component
		{
			PE_DECLARE_CLASS(PoolBall);

			PoolBall(PE::GameContext& context, PE::MemoryArena arena, PE::Handle hMyself, Events::Event_CreatePoolBall* pEvt);

			virtual void addDefaultComponents();
		};
	}; // namespace Components
}; // namespace CharacterControl


