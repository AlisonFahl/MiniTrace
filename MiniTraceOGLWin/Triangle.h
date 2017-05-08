/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma once
#include "Primitive.h"
#include "Vector3.h"
#include "Ray.h"
#include <vector>

class Triangle : public Primitive
{
private:
	Vector3 m_vertices[3];
	Vector3 m_normal;

public:
	Triangle();
	Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3);
	~Triangle();
	
	void SetTriangle(Vector3 v0, Vector3 v1, Vector3 v2);

	RayHitResult IntersectByRay(Ray& ray);
};

