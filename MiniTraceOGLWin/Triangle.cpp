/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
* 
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "Triangle.h"

Triangle::Triangle()
{
	m_vertices[0] = Vector3(-1.0, 0.0, -5.0);
	m_vertices[1] = Vector3(0.0, 1.0, -5.0);
	m_vertices[2] = Vector3(1.0, 0.0, -5.0);
	m_normal = Vector3(0.0, 0.0, 1.0);
	m_primtype = PRIMTYPE_Triangle;
}

Triangle::Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3)
{
	SetTriangle(pos1, pos2, pos3);

	m_primtype = PRIMTYPE_Triangle;
}


Triangle::~Triangle()
{
}

void Triangle::SetTriangle(Vector3 v0, Vector3 v1, Vector3 v2)
{
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;

	//Calculate Normal
	Vector3 NormalA = m_vertices[1] - m_vertices[0];
	Vector3 NormalB = m_vertices[2] - m_vertices[0];
	Vector3 Norm = NormalA.CrossProduct(NormalB);
	Norm.Normalise();
	m_normal = Norm;
}


RayHitResult Triangle::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	double t = FARFAR_AWAY;
	Vector3 intersection_point;

	auto normalDotRay = m_normal.DotProduct(ray.GetRay());
	if (normalDotRay < 0)
	{
		auto offset = -m_normal.DotProduct(m_vertices[0]);
		t = -(ray.GetRayStart().DotProduct(m_normal) + offset) / normalDotRay;

		//Calculate the exact location of the intersection using the result of t
		intersection_point = ray.GetRayStart() + ray.GetRay()*t;

		Vector3 v1, v2, n;
		float d;

		//Side 1
		v1 = m_vertices[0] - intersection_point;
		v2 = m_vertices[1] - intersection_point;
		n = v2.CrossProduct(v1);
		n.Normalise();
		d = -(ray.GetRayStart().DotProduct(n));
		if (intersection_point.DotProduct(n) + d > 0)
		{

			//Side 2
			v1 = m_vertices[1] - intersection_point;
			v2 = m_vertices[2] - intersection_point;
			n = v2.CrossProduct(v1);
			n.Normalise();
			d = -(ray.GetRayStart().DotProduct(n));
			if (intersection_point.DotProduct(n) + d > 0)
			{
				//Side 3
				v1 = m_vertices[2] - intersection_point;
				v2 = m_vertices[0] - intersection_point;
				n = v2.CrossProduct(v1);
				n.Normalise();
				d = -(ray.GetRayStart().DotProduct(n));
				if (intersection_point.DotProduct(n) + d > 0)
				{

				}
				else
				{
					t = FARFAR_AWAY;
				}
			}
			else
			{
				t = FARFAR_AWAY;
			}
		}
		else
		{
			t = FARFAR_AWAY;
		}
	}
	
	//Calculate the exact location of the intersection using the result of t
	intersection_point = ray.GetRayStart() + ray.GetRay()*t;

	if (t > 0 && t < FARFAR_AWAY) { 
		result.t = t;
		result.normal = this->m_normal;
		result.point = intersection_point;
		result.data = this;
		return result;
	}
	
	return result;
}
