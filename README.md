# Basic Physics Engine

https://github.com/Naadborole/PhysicsEngine/assets/54070209/0cfe42e5-2b18-4e9e-ab3a-80012b689b61

This is a basic pool game to demonstrate basic physics engine written in C++. This demo demonstrates 
1. Collision Detection
2. Collision Response

## Collision Detection

The physics engine has two types of collision detection algorithms working. It can detect
collision between Sphere and Sphere, Sphere and ImPlane

1. Sphere to Sphere collision
   
The sphere to sphere collision is the simplest collision detection. It simply checks if the
distance between the center of the spheres is less than the sum of thier radius.

3. Sphere to ImPlane (Infinite Mass Plane) collision

The sphere to plane collision will check if the distance of the center to the plane is less
than the radius of the sphere

## Collision Response

The collision response takes into account the impulse and conservation of momentum to calculate the response. It makes use of the following calculations.

Let $v_a$ be the velocity of Rigid body $a$ and $v_b$ be the velocity of rigid body $b$. Let $\hat{n}$ be the normal in the direction of collision, in case of sphere to sphere it the vector going between the center of sphere.
 $$v_{rel} = v_a - v_b$$

Let ${v_{rel}}^{'}$ be the new relative velocity after collision.
$${v_{rel}}^{'} * \hat{n} = \epsilon \;( v_{rel} * \hat{n} ) \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \textit{Momentum is conserved} $$
where, $\epsilon : \textit{Coefficient of restitution}$
let $J$ be the magnitude of impulse
$$J = \frac{-(1 + \epsilon)* ( v_{rel} * \hat{n} )}{\frac{1}{m_a} + \frac{1}{m_b}}$$
The final velocity of rigid bodies a and b will be given by
$$v_a^{'} = v_a + \frac{J * \hat{n}}{m_a}$$
$$v_b^{'} = v_b + \frac{J * \hat{n}}{m_b}$$
