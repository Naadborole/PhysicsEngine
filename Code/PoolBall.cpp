#include "PoolBall.h"

#include "PhysicsManager.h"
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Scene/SkeletonInstance.h"
#include "PrimeEngine/Scene/MeshInstance.h"
#include "PrimeEngine/Scene/RootSceneNode.h"

using namespace PE;
using namespace PE::Components;
using namespace CharacterControl::Events;

namespace CharacterControl {
	namespace Components {

		PE_IMPLEMENT_CLASS1(PoolBall, Component);

		PoolBall::PoolBall(PE::GameContext& context, PE::MemoryArena arena, PE::Handle hMyself, Event_CreatePoolBall* pEvt) : Component(context, arena, hMyself)
		{
			m_pContext->getGPUScreen()->AcquireRenderContextOwnership(pEvt->m_threadOwnershipMask);

			PE::Handle hSN("SCENE_NODE", sizeof(SceneNode));
			SceneNode* pMainSN = new(hSN) SceneNode(*m_pContext, m_arena, hSN);
			pMainSN->addDefaultComponents();

			pMainSN->m_base.setPos(pEvt->m_pos);
			pMainSN->m_base.setU(pEvt->m_u);
			pMainSN->m_base.setV(pEvt->m_v);
			pMainSN->m_base.setN(pEvt->m_n);
			PE::Handle hRigidBody("Pool_Rigid_Body", sizeof(PhysicsEngine::RigidBody));
			PhysicsEngine::RigidBody* pRigidBody = new(hRigidBody) PhysicsEngine::RigidBody(*m_pContext, m_arena, hRigidBody, PhysicsEngine::SPHERE);
			pRigidBody->setSphere(PhysicsEngine::Sphere(0.5));
			pRigidBody->mass = 1;
			//pRigidBody->setSphere(PhysicsEngine::Sphere(1.8/2,1.8/2));
			pRigidBody->addDefaultComponents();
			addComponent(hRigidBody);
			//Add it to PhysicsManager
			PhysicsEngine::PhysicsManager::Instance()->addRigidBody(hRigidBody);

			if(pEvt->num == 1)
			{
				pRigidBody->velocity = Vector3(-0.1,0,0);
			}
			else
			{
				pRigidBody->velocity = Vector3(0, 0, 0);
				pRigidBody->mass = 1;
			}

			RootSceneNode::Instance()->addComponent(hSN);

			// add the scene node as component of soldier without any handlers. this is just data driven way to locate scnenode for soldier's components
			{
				static int allowedEvts[] = { 0 };
				addComponent(hSN, &allowedEvts[0]);

			}

			int numskins = 1; // 8
			for (int iSkin = 0; iSkin < numskins; ++iSkin)
			{
				float z = (iSkin / 4) * 1.5f;
				float x = (iSkin % 4) * 1.5f;
				PE::Handle hSN("SCENE_NODE", sizeof(SceneNode));
				SceneNode* pSN = new(hSN) SceneNode(*m_pContext, m_arena, hSN);
				pSN->addDefaultComponents();

				pSN->m_base.setPos(Vector3(x, 0, z));

				PE::Handle hMeshInstance("MeshInstance", sizeof(MeshInstance));
				MeshInstance* pMeshInstance = new(hMeshInstance) MeshInstance(*m_pContext, m_arena, hMeshInstance);
				pMeshInstance->addDefaultComponents();

				pMeshInstance->initFromFile(pEvt->m_meshFilename, pEvt->m_package, pEvt->m_threadOwnershipMask);

				pMainSN->addComponent(hMeshInstance);

			}

			m_pContext->getGPUScreen()->ReleaseRenderContextOwnership(pEvt->m_threadOwnershipMask);
		}

		void PoolBall::addDefaultComponents()
		{
			Component::addDefaultComponents();

			// custom methods of this component
		}

	}
}