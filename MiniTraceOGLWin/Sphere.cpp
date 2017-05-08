/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include <math.h>
#include "Sphere.h"

Sphere::Sphere()
{
	//the default sphere is centred around the origin of the world coordinate
	//default radius is 2.0

	m_centre.SetZero();
	m_radius = 2.0;
	m_primtype = PRIMTYPE_Sphere;
}

Sphere::Sphere(double x, double y, double z, double r)
{
	m_centre.SetVector(x, y, z);
	m_radius = r;
	m_primtype = PRIMTYPE_Sphere;
}

Sphere::~Sphere()
{
}

RayHitResult Sphere::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;

	double t = FARFAR_AWAY;
	Vector3 normal;
	Vector3 intersection_point;

	auto originsDistance = ray.GetRayStart() - m_centre;

	auto a = ray.GetRay().Norm_Sqr();
	auto b = 2 * ray.GetRay().DotProduct(originsDistance);
	auto c = m_centre.Norm_Sqr() + ray.GetRayStart().Norm_Sqr() - (2 * (m_centre.DotProduct(ray.GetRayStart()))) - (m_radius * m_radius);

	auto discriminant = pow(b, 2) - (4 * a * c);

	if (discriminant >= 0)
	{
		auto t1 = (-b + sqrt(discriminant)) / (2 * a);
		auto t2 = (-b - sqrt(discriminant)) / (2 * a);

		t = abs(t1) < abs(t2) ? t1 : t2;
	}
	
	
	//Calculate the exact location of the intersection using the result of t
	intersection_point = ray.GetRayStart() + ray.GetRay()*t;
	
	normal = intersection_point - m_centre;
	normal.Normalise();

	if (t>0.0 && t < FARFAR_AWAY)
	{
		result.t = t;
		result.data = this;
		result.point = intersection_point;
		result.normal = normal;
	}

	return result;
}
