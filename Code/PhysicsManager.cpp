#include "PhysicsManager.h"

#include "PrimeEngine/Scene/SceneNode.h"


PhysicsEngine::PhysicsManager* PhysicsEngine::PhysicsManager::selfPoint = nullptr;


bool PhysicsEngine::PhysicsManager::checkCollision(PhysicsEngine::RigidBody* r1, PhysicsEngine::RigidBody* r2)
{
	if(r1->type == BOX)
	{
		if(r2->type == BOX)
		{
			return false;
		}
		else if(r2->type == SPHERE)
		{
			return false;
		}
	}
	else if(r1->type == SPHERE)
	{
		if(r2->type == BOX)
		{
			return false;
		}
		else if(r2->type == SPHERE)
		{
			return SphereToSphere(r1, r2);
		}
	}
	return true;
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

void PhysicsEngine::PhysicsManager::moveBall()
{
	for (UINT32 i = 0; i < Instance()->m_rigidBodies.m_size; i++)
	{
		RigidBody* ri = Instance()->m_rigidBodies[i].getObject<RigidBody>();
		if(ri->type == SPHERE)
		{
			if (ri->velocity.length() != 0) {
				ri->getBase()->setPos(ri->getBase()->getPos() + ri->velocity);
			}
		}
	}
}

float PhysicsEngine::PhysicsManager::getIntersectionDistance(RigidBody* a, RigidBody* b)
{
	Matrix4x4* baseA = a->getBase();
	Matrix4x4* baseB = b->getBase();
	Vector3 centerA = baseA->getPos();
	Vector3 centerB = baseB->getPos();
	return (a->sph.radius + b->sph.radius) - (centerB - centerA).length();
}


void PhysicsEngine::PhysicsManager::checkCollision()
{
	for (UINT32 i = 0; i < Instance()->m_rigidBodies.m_size; i++)
	{
		RigidBody* ri = Instance()->m_rigidBodies[i].getObject<RigidBody>();
		
		for (UINT32 j = i+1; j < Instance()->m_rigidBodies.m_size; j++)
		{
			RigidBody* rj = Instance()->m_rigidBodies[j].getObject<RigidBody>();
			if(checkCollision(ri,rj))
			{
				float Idist = getIntersectionDistance(ri, rj);
				Vector3 v_rel = ri->velocity - rj->velocity;
				Vector3 n = rj->getBase()->getPos() - ri->getBase()->getPos();
				n.normalize();
				ri->getBase()->setPos(ri->getBase()->getPos() + (-Idist) * n);
				float J = ((-1 - 0.6)*v_rel.dotProduct(n)) / ((1 / ri->mass) + (1 / rj->mass));
				Vector3 v_i = ri->velocity + (J / ri->mass) * n;
				Vector3 v_j = rj->velocity - (J / rj->mass) * n;
				ri->velocity = v_i;
				rj->velocity = v_j;
			}
		}
	}
}

bool PhysicsEngine::PhysicsManager::SphereToSphere(RigidBody* a, RigidBody* b)
{
	Matrix4x4* baseA = a->getBase();
	Matrix4x4* baseB = b->getBase();
	Vector3 centerA = baseA->getPos();
	Vector3 centerB = baseB->getPos();
	float length = (centerB - centerA).length();
	float distAddRadius = a->sph.radius + b->sph.radius;
	return length < (distAddRadius);
}

bool isSeperate(Vector3& RPos, Vector3 Plane, PhysicsEngine::RigidBody* a, PhysicsEngine::RigidBody* b)
{
	return (fabs(RPos* Plane) >
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

