#pragma once

namespace PhysicsEngine
{
	struct Sphere
	{
		float radius;
		float centerOffset; //Offset in direction of v
		Sphere()
		{
			radius = 0;
		}
		Sphere(float rad){
			radius = rad;
			centerOffset = 0;
		}
		Sphere(float rad, float co) {
			radius = rad;
			centerOffset = co;
		}
	};
}
