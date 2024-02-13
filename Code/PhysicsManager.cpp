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
					if(ri->getHandle().getDbgName() == "Soldier_Rigid_Body" && rj->getHandle().getDbgName() == "Tank_Rigid_Body")
					{
						int length = ri->sph.radius + rj->sph.radius+0.5;
						Vector3 dir = (ri->getBase()->getPos() - rj->getBase()->getPos());
						dir.normalize();
						ri->getBase()->setPos(rj->getBase()->getPos() + length * dir);

					}
					else if(rj->getHandle().getDbgName() == "Soldier_Rigid_Body" && ri->getHandle().getDbgName() == "Tank_Rigid_Body")
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
				if(ri->getHandle().getDbgName() == "Soldier_Rigid_Body" && !AABBToAABB(ri, Instance()->Ground))
				{
					ri->getBase()->setPos(ri->getBase()->getPos() - ri->getBase()->getV());
					continue;
				}
				if(rj->getHandle().getDbgName() == "Soldier_Rigid_Body" && !AABBToAABB(rj, Instance()->Ground))
				{
					rj->getBase()->setPos(rj->getBase()->getPos() - ri->getBase()->getV());
					continue;
				}
				if (AABBToAABB(ri, rj)) {
					if (ri->getHandle().getDbgName() == "Soldier_Rigid_Body" && rj->getHandle().getDbgName() == "Tank_Rigid_Body")
					{
						ri->getBase()->setPos(ri->PrevPos + 0.1*ri->getBase()->getU() - 0.05*ri->getBase()->getN());
					}
					else if (rj->getHandle().getDbgName() == "Soldier_Rigid_Body" && ri->getHandle().getDbgName() == "Tank_Rigid_Body")
					{
						rj->getBase()->setPos(rj->PrevPos + 0.1*rj->getBase()->getU() - 0.05* ri->getBase()->getN());
					}
				}
			}
		}
	}
}

bool PhysicsEngine::PhysicsManager::SAT(RigidBody a, RigidBody b)
{
	Vector3 pointsA[8];
	Vector3 pointsB[8];
	PE::Handle hParent = a.getFirstParentByType<PE::Components::Component>();
	PE::Components::SceneNode* aSN = hParent.getObject<PE::Components::Component>()->getFirstComponent<PE::Components::SceneNode>();
	if (!aSN)
	{
		aSN = hParent.getObject<PE::Components::SceneNode>();
	}
	PE::Handle hParent2 = b.getFirstParentByType<PE::Components::Component>();
	PE::Components::SceneNode* bSN = hParent2.getObject<PE::Components::Component>()->getFirstComponent<PE::Components::SceneNode>();
	if (!bSN)
	{
		bSN = hParent2.getObject<PE::Components::SceneNode>();
	}
	a.aabb.getBoundingPoints(aSN->m_base.getPos(), pointsA, aSN->m_base.getU(), aSN->m_base.getV(), aSN->m_base.getN());
	b.aabb.getBoundingPoints(bSN->m_base.getPos(), pointsB, bSN->m_base.getU(), bSN->m_base.getV(), bSN->m_base.getN());

	//Edges are 0-1, 1-2, 2-3, 3-0,
	return false;
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

