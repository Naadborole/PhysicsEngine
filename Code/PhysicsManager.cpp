#include "PhysicsManager.h"


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
		for (UINT32 j = 0; j < Instance()->m_rigidBodies.m_size; j++)
		{
			if(i == j)
				continue;
			RigidBody* rj = Instance()->m_rigidBodies[j].getObject<RigidBody>();
			
		}
	}
}

