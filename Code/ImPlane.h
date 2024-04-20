#pragma once
#include "PrimeEngine/Math/Vector3.h"

namespace PhysicsEngine
{
	struct ImPlane
	{
		Vector3 p;
		Vector3 n;
		ImPlane()
		{
			p = Vector3(0, 0, 0);
			n = Vector3(0, 0, 0);
		}
		ImPlane(Vector3 point, Vector3 normal) {
			normal.normalize();
			p = point;
			n = normal;
		}
	};
}
