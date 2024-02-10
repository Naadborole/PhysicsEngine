#include "RigidBody.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Scene/DebugRenderer.h"
#include "PrimeEngine/Scene/SceneNode.h"

namespace PhysicsEngine
{

	PE_IMPLEMENT_CLASS1(RigidBody, Component);

	void RigidBody::addDefaultComponents()
	{
		Component::addDefaultComponents();
	}

	void RigidBody::setBoundingBox(AABB bb)
	{
		aabb = bb;
	}

	void RigidBody::setSphere(Sphere s)
	{
		sph = s;
	}


	void RigidBody::drawBoundingBox()
	{
		PE::Handle hParent = getFirstParentByType<Component>();
		if (hParent.isValid())
		{
			// see if parent has scene node component
			PE::Components::SceneNode* SN = hParent.getObject<Component>()->getFirstComponent<PE::Components::SceneNode>();
			if(!SN)
			{
				SN = hParent.getObject<PE::Components::SceneNode>();
			}
			if(SN)
			{
				Vector3 linepts[48];
				Vector3 color(1.f, 0.0f, 1.f);
				Vector3 pos = SN->m_base.getPos();
				Vector3 n = SN->m_base.getN();
				Vector3 u = SN->m_base.getU();
				Vector3 v = SN->m_base.getV();


				/*linepts[0] = pos;
				linepts[1] = color;
				linepts[2] = pos + 100 * v;
				linepts[3] = color;*/
				int ipts = -1;
				//-----------Lower Face----------------------
				linepts[++ipts] = pos + aabb.Nex * n + aabb.Uex * u;//0
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n + aabb.Uex * u;//1
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n + aabb.Uex * u;//1
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n - aabb.Uex * u;//2
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n - aabb.Uex * u;//2
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n - aabb.Uex * u;//3
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n - aabb.Uex * u;//3
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n + aabb.Uex * u;//0
				linepts[++ipts] = color;
				//-----------------------------------------------
				//-----------Upper Face----------------------
				linepts[++ipts] = pos + aabb.Nex * n + aabb.Uex * u + aabb.Vex*v;//4
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n + aabb.Uex * u + aabb.Vex * v;//5
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n + aabb.Uex * u + aabb.Vex * v;//5
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n - aabb.Uex * u + aabb.Vex * v;//6
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n - aabb.Uex * u + aabb.Vex * v;//6
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n - aabb.Uex * u + aabb.Vex * v;//7
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n - aabb.Uex * u + aabb.Vex * v;//7
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n + aabb.Uex * u + aabb.Vex * v;//4
				linepts[++ipts] = color;
				//-----------------------------------------------------
				linepts[++ipts] = pos + aabb.Nex * n + aabb.Uex * u;//0
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n + aabb.Uex * u + aabb.Vex * v;//4
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n + aabb.Uex * u;//1
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n + aabb.Uex * u + aabb.Vex * v;//5
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n - aabb.Uex * u;//2
				linepts[++ipts] = color;
				linepts[++ipts] = pos - aabb.Nex * n - aabb.Uex * u + aabb.Vex * v;//6
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n - aabb.Uex * u;//3
				linepts[++ipts] = color;
				linepts[++ipts] = pos + aabb.Nex * n - aabb.Uex * u + aabb.Vex * v;//7
				linepts[++ipts] = color;

				PE::Components::DebugRenderer::Instance()->createLineMesh(false, Matrix4x4(), &linepts[0].m_x, 24, 0);
			}
		}
	}

}

