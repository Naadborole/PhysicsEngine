#pragma once
#include "PrimeEngine/Math/Vector3.h"
#include "RigidBody.h"

namespace PhysicsEngine
{
	struct CollisionInfo
	{
		RigidBody* b1;
		RigidBody* b2;
		Vector3 pointOfContact;
		float intersectionDistance;
		Vector3 surfaceNormal;

		CollisionInfo(RigidBody* r1, RigidBody* r2, Vector3 poc, float dist, Vector3 sn)
		{
			b1 = r1;
			b2 = r2;
			pointOfContact = poc;
			intersectionDistance = dist;
			surfaceNormal = sn;
		}
	};
}
