#pragma once
#include "PrimeEngine/Math/Vector3.h"

namespace PhysicsEngine
{
	struct ImPlane
	{
		Vector3 p;
		Vector3 n; //Offset in direction of v
		ImPlane(Vector3 point, Vector3 normal) {
			normal.normalize();
			p = point;
			n = normal;
		}
	};
}
