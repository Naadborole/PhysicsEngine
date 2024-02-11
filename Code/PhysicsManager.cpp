#include "PhysicsManager.h"

#include "PrimeEngine/Scene/SceneNode.h"


PhysicsEngine::PhysicsManager* PhysicsEngine::PhysicsManager::selfPoint = nullptr;


bool PhysicsEngine::PhysicsManager::checkCollision(PhysicsEngine::RigidBody* r1, PhysicsEngine::RigidBody* r2)
{
	return false;
}

PhysicsEngine::PhysicsManager::PhysicsManager()
{
	m_rigidBodies.reset(512);
	selfPoint = nullptr;
}

void PhysicsEngine::PhysicsManager::drawBoundingBox()
{
	for(UINT32 i=0; i<Instance()->m_rigidBodies.m_size; i++)
	{
		RigidBody* t = Instance()->m_rigidBodies[i].getObject<RigidBody>();
		if(t->type == ShapeType::BOX)
		{
			t->drawBoundingBox();
		}
	}
}

void PhysicsEngine::PhysicsManager::checkCollision()
{
	for (UINT32 i = 0; i < Instance()->m_rigidBodies.m_size; i++)
	{
		RigidBody* ri = Instance()->m_rigidBodies[i].getObject<RigidBody>();
		for (UINT32 j = i+1; j < Instance()->m_rigidBodies.m_size; j++)
		{
			RigidBody* rj = Instance()->m_rigidBodies[j].getObject<RigidBody>();
			if(ri->type == SPHERE && rj->type == SPHERE)
			{
				if (AABBToAABB(ri, rj))
				{
					if(ri->getHandle().getDbgName() == "Soldier_Rigid_Body")
					{
						int length = ri->sph.radius + rj->sph.radius+0.5;
						Vector3 dir = (ri->getBase()->getPos() - rj->getBase()->getPos());
						dir.normalize();
						ri->getBase()->setPos(rj->getBase()->getPos() + length * dir);

					}
					else if(rj->getHandle().getDbgName() == "Soldier_Rigid_Body")
					{
						int length = rj->sph.radius + ri->sph.radius+1;
						Vector3 dir = (rj->getBase()->getPos() - ri->getBase()->getPos());
						dir.normalize();
						rj->getBase()->setPos(ri->getBase()->getPos() + length * dir);
					}
				}
					
			}
			else if (ri->type == BOX && rj->type == BOX)
			{
				if (AABBToAABB(ri, rj)) {
					if (ri->getHandle().getDbgName() == "Soldier_Rigid_Body")
					{
						int length = 4;
						Vector3 dir = (ri->getBase()->getPos() - rj->getBase()->getPos());
						dir.normalize();
						ri->getBase()->setPos(rj->getBase()->getPos() + length * dir);

					}
					else if (rj->getHandle().getDbgName() == "Soldier_Rigid_Body")
					{
						int length = 4;
						Vector3 dir = (rj->getBase()->getPos() - ri->getBase()->getPos());
						dir.normalize();
						rj->getBase()->setPos(ri->getBase()->getPos() + length * dir);
					}
				}
			}
			
		}
	}
}

bool PhysicsEngine::PhysicsManager::SphereToSphere(RigidBody* a, RigidBody* b)
{
	Matrix4x4* baseA = a->getBase();
	Matrix4x4* baseB = b->getBase();
	Vector3 centerA = baseA->getPos() + a->sph.centerOffset* baseA->getV();
	Vector3 centerB = baseB->getPos() + b->sph.centerOffset * baseB->getV();
	float length = (centerB - centerA).length();
	float center = a->sph.radius + b->sph.radius;
	return (centerB - centerA).length() < (a->sph.radius + b->sph.radius);
}

bool isSeperate(Vector3& RPos, Vector3 Plane, PhysicsEngine::RigidBody* a, PhysicsEngine::RigidBody* b)
{
	return (fabs(RPos * Plane) >
		(fabs((a->getBase()->getN() * a->aabb.Nex) * Plane) +
			fabs((a->getBase()->getV() * a->aabb.Vex) * Plane) +
			fabs((a->getBase()->getU() * a->aabb.Uex) * Plane) +
			fabs((b->getBase()->getN() * b->aabb.Nex) * Plane) +
			fabs((b->getBase()->getV() * b->aabb.Vex) * Plane) +
			fabs((b->getBase()->getU() * b->aabb.Uex) * Plane)));
}

bool PhysicsEngine::PhysicsManager::AABBToAABB(RigidBody* a, RigidBody* b)
{
	//SAT
	static Vector3 RPos;
	Vector3 aCent = a->getBase()->getPos() + (a->aabb.Vex / 2) * a->getBase()->getV();
	Vector3 bCent = b->getBase()->getPos() + (b->aabb.Vex / 2) * b->getBase()->getV();
	RPos = aCent - bCent;

	return !(isSeperate(RPos, a->getBase()->getN(), a, b) ||
		isSeperate(RPos, a->getBase()->getV(), a, b) ||
		isSeperate(RPos, a->getBase()->getU(), a, b) ||
		isSeperate(RPos, b->getBase()->getN(), a, b) ||
		isSeperate(RPos, b->getBase()->getV(), a, b) ||
		isSeperate(RPos, b->getBase()->getU(), a, b) ||
		isSeperate(RPos, a->getBase()->getN().crossProduct( b->getBase()->getN()), a, b) ||
		isSeperate(RPos, a->getBase()->getN().crossProduct ( b->getBase()->getV()), a, b) ||
		isSeperate(RPos, a->getBase()->getN().crossProduct( b->getBase()->getU()), a, b) ||
		isSeperate(RPos, a->getBase()->getV().crossProduct( b->getBase()->getN()), a, b) ||
		isSeperate(RPos, a->getBase()->getV().crossProduct( b->getBase()->getV()), a, b) ||
		isSeperate(RPos, a->getBase()->getV().crossProduct( b->getBase()->getU()), a, b) ||
		isSeperate(RPos, a->getBase()->getU().crossProduct( b->getBase()->getN()), a, b) ||
		isSeperate(RPos, a->getBase()->getU().crossProduct( b->getBase()->getV()), a, b) ||
		isSeperate(RPos, a->getBase()->getU().crossProduct( b->getBase()->getU()), a, b));
}

