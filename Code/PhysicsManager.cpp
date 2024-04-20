#include "PhysicsManager.h"

#include "PrimeEngine/Scene/SceneNode.h"


PhysicsEngine::PhysicsManager* PhysicsEngine::PhysicsManager::selfPoint = nullptr;


bool PhysicsEngine::PhysicsManager::doesfallinHole(Vector3 pos)
{
	float dis = (Instance()->CornerA - pos).length();
	if (dis < 2)
		return true;
	dis = (Instance()->CornerB - pos).length();
	if (dis < 2)
		return true;
	dis = (Instance()->CornerC - pos).length();
	if (dis < 2)
		return true;
	dis = (Instance()->CornerD - pos).length();
	if (dis < 2)
		return true;
	return false;
}

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
		else if(r2->type == IMPLANE)
		{
			return SphereToPlane(r1, r2);
		}
	}
	else if(r1->type == IMPLANE)
	{
		if(r2->type == SPHERE)
		{
			return SphereToPlane(r2, r1);
		}
	}
	return false;
}

PhysicsEngine::PhysicsManager::PhysicsManager()
{
	m_rigidBodies.reset(512);
	selfPoint = nullptr;
}

void PhysicsEngine::PhysicsManager::setCorner(int id, Vector3 pos)
{
	if (id == 100)
		Instance()->CornerA = pos;
	else if (id == 101)
		Instance()->CornerB = pos;
	else if (id == 102)
		Instance()->CornerC = pos;
	else
		Instance()->CornerD = pos;
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

void PhysicsEngine::PhysicsManager::moveZeroBall(Vector3 camPos, Vector3 camN)
{
	if (Instance()->zeroBall == nullptr)
		return;
	Vector3 dir = Instance()->zeroBall->getBase()->getPos() - camPos;
	dir = Vector3(dir.getX(), 0, dir.getZ());
	dir.normalize();
	Instance()->zeroBall->velocity = 0.2 * dir;
}

Vector3 PhysicsEngine::PhysicsManager::getPerpendicular(Vector3 A)
{
	return A.crossProduct(Vector3(0, 1, 0));
}


void PhysicsEngine::PhysicsManager::moveBall()
{
	for (UINT32 i = 0; i < Instance()->m_rigidBodies.m_size; i++)
	{
		RigidBody* ri = Instance()->m_rigidBodies[i].getObject<RigidBody>();
		if(ri->type == SPHERE)
		{
			if (ri->velocity.length() != 0) {
				Vector3 perp = getPerpendicular(ri->velocity);
				perp.normalize();
				ri->getBase()->setPos(ri->getBase()->getPos() + ri->velocity);
				ri->getBase()->turnAboutAxis(ri->velocity.length(), perp);
				ri->velocity = 0.998 * (ri->velocity);
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
				if (ri->type == SPHERE && rj->type == SPHERE) {
					float Idist = getIntersectionDistance(ri, rj);
					Vector3 v_rel = ri->velocity - rj->velocity;
					Vector3 n = rj->getBase()->getPos() - ri->getBase()->getPos();
					n.normalize();
					ri->getBase()->setPos(ri->getBase()->getPos() + (-Idist) * n);
					float J = ((-1 - 0.6) * v_rel.dotProduct(n)) / ((1 / ri->mass) + (1 / rj->mass));
					Vector3 v_i = ri->velocity + (J / ri->mass) * n;
					Vector3 v_j = rj->velocity - (J / rj->mass) * n;
					ri->velocity = v_i;
					rj->velocity = v_j;
				}
				else if((ri->type == SPHERE && rj->type == IMPLANE) || (rj->type == SPHERE && ri->type == IMPLANE))
				{
					RigidBody* rs, * rl;
					if(ri->type == SPHERE)
					{
						rs = ri;
						rl = rj;
					}
					else
					{
						rs = rj;
						rl = ri;
					}
					float dist = rs->sph.radius - (rs->getBase()->getPos() - rl->pln.p).dotProduct(rl->pln.n);
					rs->getBase()->setPos(rs->getBase()->getPos() + (dist) * rl->pln.n);
					Vector3 reflected = 2*(rs->velocity.dotProduct(rl->pln.n))*rl->pln.n;
					rs->velocity -= reflected;
					rs->velocity = 0.8 * (rs->velocity);
				}
			}
		}
		if (ri->type == SPHERE) {
			Vector3 pos = ri->getBase()->getPos();
			if (doesfallinHole(pos))
			{
				ri->velocity = Vector3(0, -2, 0);
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

bool PhysicsEngine::PhysicsManager::SphereToPlane(RigidBody* rs, RigidBody* rl)
{
	float dist = (rs->getBase()->getPos() - rl->pln.p).dotProduct(rl->pln.n);
	return dist < rs->sph.radius;

}