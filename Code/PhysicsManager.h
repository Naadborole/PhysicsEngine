#ifndef _PHYSICS_ENGINE_
#define _PHYSICS_ENGINE_
#include "RigidBody.h"
#include "PrimeEngine/Events/Component.h"


namespace PhysicsEngine
{


	class PhysicsManager
	{
	public:
		PhysicsManager();

		static void Construct()
		{
			selfPoint = new PhysicsManager();
		};

		static PhysicsManager* Instance()
		{
			return selfPoint;
		}

		void addRigidBody(PE::Handle r)
		{
			m_rigidBodies.add(r);
		}
		static void drawBoundingBox();
		static void checkCollision();
		bool checkCollision(PhysicsEngine::RigidBody* r1, PhysicsEngine::RigidBody* r2);
		Array<PE::Handle, 1> m_rigidBodies;
		static PhysicsManager* selfPoint;
		static bool SAT(RigidBody a, RigidBody b);
		static bool SphereToSphere(RigidBody* a, RigidBody* b);
		static bool AABBToAABB(RigidBody* a, RigidBody* b);
		RigidBody* Ground;
	};
}
#endif
