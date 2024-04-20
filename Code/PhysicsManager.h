#ifndef _PHYSICS_ENGINE_
#define _PHYSICS_ENGINE_
#include "CollisionInfo.h"
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
			selfPoint->zeroBall = NULL;
			selfPoint->Ground = NULL;
			selfPoint->CornerA = Vector3(0, 0, 0);
			selfPoint->CornerB = Vector3(0, 0, 0);
			selfPoint->CornerC = Vector3(0, 0, 0);
			selfPoint->CornerD = Vector3(0, 0, 0);
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
		static bool checkCollision(PhysicsEngine::RigidBody* r1, PhysicsEngine::RigidBody* r2);
		static void moveBall();
		static Array<CollisionInfo> createCollList();
		Array<PE::Handle, 1> m_rigidBodies;
		static PhysicsManager* selfPoint;
		static bool SAT(RigidBody a, RigidBody b);
		static bool SphereToSphere(RigidBody* a, RigidBody* b);
		static bool AABBToAABB(RigidBody* a, RigidBody* b);
		static float getIntersectionDistance(RigidBody *a, RigidBody *b);
		static bool SphereToPlane(RigidBody* rs, RigidBody* rl);
		static void moveZeroBall(Vector3 camPos, Vector3 camN);
		static Vector3 getPerpendicular(Vector3);
		static void setCorner(int id, Vector3 pos);
		static bool doesfallinHole(Vector3 pos);
		RigidBody* Ground;
		RigidBody* zeroBall;
		Vector3 CornerA;
		Vector3 CornerB;
		Vector3 CornerC;
		Vector3 CornerD;
	};
}
#endif
