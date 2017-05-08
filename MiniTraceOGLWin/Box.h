/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma	once
#include "Primitive.h"
#include "Vector3.h"
#include "Triangle.h"
#include "Material.h"
#include "Texture.h"

class Box : public Primitive
{
	private:
		Triangle m_triangles[12];

		Texture* m_Texture;
		Vector3 m_Position;
		double m_Width, m_Height;

	public:
		Box();
		Box(Vector3 position, double width, double height, double depth);
		~Box();

		void SetBox(Vector3 position, double width, double height, double depth);

		RayHitResult IntersectByRay(Ray& ray);
		Colour GetTextureColour(Vector3& position);

};

