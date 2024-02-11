#pragma once
#include "PrimeEngine/Math/Vector3.h"

namespace PhysicsEngine
{
	struct AABB
	{
		float Uex, Vex, Nex;
		AABB()
		{
			Uex = 0;
			Vex = 0;
			Nex = 0;
		}
		AABB(float ux, float vx, float nx)
		{
			Uex = ux;
			Vex = vx;
			Nex = nx;
		}
		Vector3* getBoundingPoints(Vector3 pos, Vector3 points[], Vector3 u, Vector3 v, Vector3 n);
	};
}
