#pragma once
#include "AABB.h"
#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Math/Vector3.h"
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "ImPlane.h"
#include "Sphere.h"
#include "PrimeEngine/Math/Matrix4x4.h"
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"



namespace PhysicsEngine
{
	enum ShapeType{BOX, SPHERE, IMPLANE};

	struct RigidBody : public PE::Components::Component
	{
		PE_DECLARE_CLASS(RigidBody);

	public:
		Vector3 PrevPos; 
		Vector3 destPos; 
		float mass;
		Vector3 velocity;

		AABB aabb;
		Sphere sph;
		ImPlane pln;
		ShapeType type;
		RigidBody(PE::GameContext& context, PE::MemoryArena arena, PE::Handle hMyself, ShapeType t) : Component(
			context, arena, hMyself)
		{
			type = t;
		}
		RigidBody(PE::GameContext& context, PE::MemoryArena arena, PE::Handle hMyself, ShapeType t, Sphere s) : Component(
			context, arena, hMyself)
		{
			type = t;
			sph = s;
		}
		RigidBody(PE::GameContext& context, PE::MemoryArena arena, PE::Handle hMyself, ShapeType t, AABB ab) : Component(
			context, arena, hMyself)
		{
			type = t;
			aabb = ab;
		}
		Vector3 getVelocityNormal();
		void setBoundingBox(AABB aabb);
		void setSphere(Sphere s);
		virtual void addDefaultComponents();
		void updatePos(Vector3 currPos, Vector3 destPos);
		void drawBoundingBox();
		Matrix4x4* getBase();
	};
}
