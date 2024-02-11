#include "AABB.h"

namespace PhysicsEngine
{
	Vector3* AABB::getBoundingPoints(Vector3 pos, Vector3 linepts[], Vector3 u, Vector3 v, Vector3 n)
	{
		int ipts = -1;
		linepts[++ipts] = pos + Nex * n + Uex * u;//0
		linepts[++ipts] = pos - Nex * n + Uex * u;//1
		linepts[++ipts] = pos - Nex * n - Uex * u;//2
		linepts[++ipts] = pos + Nex * n - Uex * u;//3
		linepts[++ipts] = pos + Nex * n + Uex * u + Vex * v;//4
		linepts[++ipts] = pos - Nex * n + Uex * u + Vex * v;//5
		linepts[++ipts] = pos - Nex * n - Uex * u + Vex * v;//6
		linepts[++ipts] = pos + Nex * n - Uex * u + Vex * v;//7
		return linepts;
	}

}
