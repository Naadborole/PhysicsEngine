#pragma once
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
	};
}
