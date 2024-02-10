#pragma once
#include "AABB.h"
#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Math/Vector3.h"
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"



namespace PhysicsEngine
{
	enum ShapeType{BOX, SPHERE};

	struct RigidBody : public PE::Components::Component
	{
		PE_DECLARE_CLASS(RigidBody);

	private:
		Vector3 currPos; //current position of the object
		Vector3 destPos; //Destination position of object
		AABB aabb;
	public:
		ShapeType type;
		RigidBody(PE::GameContext& context, PE::MemoryArena arena, PE::Handle hMyself, ShapeType t) : Component(
			context, arena, hMyself)
		{
			type = t;
		}
		void setBoundingBox(AABB aabb);
		virtual void addDefaultComponents();
		void updatePos(Vector3 currPos, Vector3 destPos);
		void drawBoundingBox();
	};
}