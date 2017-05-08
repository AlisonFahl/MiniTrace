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

#ifdef WIN32
#include <Windows.h>
#include <gl/GL.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"
#include "Camera.h"
#include "Box.h"

RayTracer::RayTracer()
{
	m_buffHeight = m_buffWidth = 0.0;
	m_renderCount = 0;
	SetTraceLevel(5);
	m_traceflag = (TraceFlag)(TRACE_AMBIENT | TRACE_DIFFUSE_AND_SPEC |
		TRACE_SHADOW | TRACE_REFLECTION | TRACE_REFRACTION);
}

RayTracer::RayTracer(int Width, int Height)
{
	m_buffWidth = Width;
	m_buffHeight = Height;
	m_renderCount = 0;
	SetTraceLevel(5);
	
	m_traceflag = (TraceFlag)(TRACE_AMBIENT | TRACE_DIFFUSE_AND_SPEC |
		TRACE_SHADOW | TRACE_REFLECTION | TRACE_REFRACTION);
}

RayTracer::~RayTracer()
{

}

void RayTracer::DoRayTrace( Scene* pScene )
{
	Camera* cam = pScene->GetSceneCamera();
	
	Vector3 camRightVector = cam->GetRightVector();
	Vector3 camUpVector = cam->GetUpVector();
	Vector3 camViewVector = cam->GetViewVector();
	Vector3 centre = cam->GetViewCentre();
	Vector3 camPosition = cam->GetPosition();

	double sceneWidth = pScene->GetSceneWidth();
	double sceneHeight = pScene->GetSceneHeight();

	double pixelDX = sceneWidth / m_buffWidth;
	double pixelDY = sceneHeight / m_buffHeight;
	
	int total = m_buffHeight*m_buffWidth;
	int done_count = 0;
	
	Vector3 start;

	start[0] = centre[0] - ((sceneWidth * camRightVector[0])
		+ (sceneHeight * camUpVector[0])) / 2.0;
	start[1] = centre[1] - ((sceneWidth * camRightVector[1])
		+ (sceneHeight * camUpVector[1])) / 2.0;
	start[2] = centre[2] - ((sceneWidth * camRightVector[2])
		+ (sceneHeight * camUpVector[2])) / 2.0;
	
	Colour scenebg = pScene->GetBackgroundColour();

	if (m_renderCount == 0)
	{
		fprintf(stdout, "Trace start.\n");

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < m_buffHeight; i++) {
			for (int j = 0; j < m_buffWidth; j++) {

				//calculate the metric size of a pixel in the view plane (e.g. framebuffer)
				Vector3 pixel;

				pixel[0] = start[0] + (i + 0.5) * camUpVector[0] * pixelDY
					+ (j + 0.5) * camRightVector[0] * pixelDX;
				pixel[1] = start[1] + (i + 0.5) * camUpVector[1] * pixelDY
					+ (j + 0.5) * camRightVector[1] * pixelDX;
				pixel[2] = start[2] + (i + 0.5) * camUpVector[2] * pixelDY
					+ (j + 0.5) * camRightVector[2] * pixelDX;

				/*
				* setup view ray
				* In perspective projection, each view ray originates from the eye (camera) position 
				* and pierces through a pixel in the view plane
				*
				* TODO: For a little extra credit, set up the view rays to produce orthographic projection
				*/
				Ray viewray;
				viewray.SetRay(camPosition,	(pixel - camPosition).Normalise());
				
				//trace the scene using the view ray
				//the default colour is the background colour, unless something is hit along the way
				Colour colour = this->TraceScene(pScene, viewray, scenebg, m_traceLevel);

				/*
				* The only OpenGL code we need
				* Draw the pixel as a coloured rectangle
				*/
				glColor3f(colour.red, colour.green, colour.blue);
				glRecti(j, i, j + 1, i + 1);
			}
			glFlush();
		}

		fprintf(stdout, "Done!!!\n");
		m_renderCount++;
	}
	glFlush();
}

Colour RayTracer::TraceScene(Scene* pScene, Ray& ray, Colour incolour, int tracelevel, bool shadowray)
{
	RayHitResult result;
	Colour outcolour = incolour;
	std::vector<Light*>* light_list = pScene->GetLightList();

	if (tracelevel <= 0) // reach the MAX depth of the recursion.
	{
		return outcolour;
	}

	result = pScene->IntersectByRay(ray, shadowray);

	if (result.data) //the ray has hit something
	{
		if (shadowray)
		{
			outcolour.red *= 0.1;
			outcolour.green *= 0.1;
			outcolour.blue *= 0.1;

			return outcolour;
		}
		else
		{
			Vector3 start = ray.GetRayStart();
			outcolour = CalculateLighting(light_list,
				&start,
				&result);
		}

		
		
		if(m_traceflag & TRACE_REFLECTION)
		{
			//Only consider reflection for spheres and boxes
			if (((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Sphere ||
				((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Box)
			{
				auto reflection_vec = ray.GetRay().Reflect(result.normal);
				reflection_vec.Normalise();

				//Add an offset to the point to avoid intersection with the point's primitive
				auto start_point = result.point + (reflection_vec * 0.00001);
				Ray reflection_ray;
				reflection_ray.SetRay(start_point, reflection_vec);

				auto reflected_color = TraceScene(pScene, reflection_ray, outcolour, tracelevel-1, false);
				outcolour.red += (reflected_color.red * 0.18);
				outcolour.green += (reflected_color.green * 0.18);
				outcolour.blue += (reflected_color.blue * 0.18);
			}
		}

		if (m_traceflag & TRACE_REFRACTION)
		{
			//Only consider refraction for spheres and boxes
			if (((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Sphere ||
				((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Box)
			{
				auto light_vec = result.point - ray.GetRayStart();
				light_vec.Normalise();

				auto refraction_vec = light_vec.Refract(result.normal, 0.96/*Air-Glass refraction*/);

				Ray refraction_ray;
				refraction_ray.SetRay(result.point + (refraction_vec * 0.00001), refraction_vec);
				auto refraction_color = TraceScene(pScene, refraction_ray, outcolour, tracelevel - 1, false);

				outcolour.red = (outcolour.red * 0.7) + (refraction_color.red * 0.3);
				outcolour.green = (outcolour.green * 0.7) + (refraction_color.green * 0.3);
				outcolour.blue = (outcolour.blue * 0.7) + (refraction_color.blue * 0.3);
			}
		}
		
		//////Check if this is in shadow
		if ( m_traceflag & TRACE_SHADOW)
		{
			std::vector<Light*>::iterator lit_iter = light_list->begin();
			while (lit_iter != light_list->end())
			{
				auto light_pos = (*lit_iter)->GetLightPosition();
				auto light_vec = light_pos - result.point;
				light_vec.Normalise();
				//Add an offset to the point to avoid intersection with the point's primitive
				auto start_point = result.point + (light_vec * 0.00001);
				Ray shadow_ray;
				shadow_ray.SetRay(start_point, light_vec);
				outcolour = TraceScene(pScene, shadow_ray, outcolour, tracelevel-1, true);

				lit_iter++;
			}
		}
	}
		
	return outcolour;
}

Colour RayTracer::CalculateLighting(std::vector<Light*>* lights, Vector3* campos, RayHitResult* hitresult)
{
	Colour outcolour;
	std::vector<Light*>::iterator lit_iter = lights->begin();

	//Retrive the material for the intersected primitive
	Primitive* prim = (Primitive*)hitresult->data;
	Material* mat = prim->GetMaterial();

	//the default output colour is the ambient colour
	outcolour = mat->GetAmbientColour();
	
	//This is a hack to set a checker pattern on the planes
	//Do not modify it
	if (((Primitive*)hitresult->data)->m_primtype == Primitive::PRIMTYPE_Plane)
	{
		int dx = (hitresult->point[0]/2.0);
		int dy = (hitresult->point[1]/2.0);
		int dz = (hitresult->point[2]/2.0);

		if (dx % 2 || dy % 2 || dz % 2)
		{
			outcolour.red = 1.0;
			outcolour.green = 1.0;
			outcolour.blue = 1.0;

		}
		else
		{
			outcolour.red = 0.0;
			outcolour.green = 0.0;
			outcolour.blue = 0.0;
		}
	}

	////Go through all the light sources in the scene
	//and calculate the lighting at the intersection point
	if (m_traceflag & TRACE_DIFFUSE_AND_SPEC)
	{
		while (lit_iter != lights->end())
		{
			Vector3 light_pos = (*lit_iter)->GetLightPosition();  //position of the light source
			Vector3 normal = hitresult->normal; //surface normal at intersection
			Vector3 surface_point = hitresult->point; //location of the intersection on the surface
			

			Colour diffuse_color = { 0 };
			Colour specular_color = { 0 };

			//Light Direction
			auto light_direction = light_pos - surface_point;
			light_direction.Normalise();
			
			//Get Diffuse Colour
			auto diffuse_intensity = normal.DotProduct(light_direction);
			if (diffuse_intensity > 0)
			{
				//Set Diffuse Colour
				diffuse_color = mat->GetDiffuseColour();
				diffuse_color.red *= diffuse_intensity;
				diffuse_color.green *= diffuse_intensity;
				diffuse_color.blue *= diffuse_intensity;

				//Get Specular Phong Model
				auto reflection = (surface_point - light_pos).Reflect(normal);
				reflection.Normalise();

				auto eye_vector = (*campos) - surface_point;
				eye_vector.Normalise();

				auto spec_value = eye_vector.DotProduct(reflection);
				if (spec_value > 0)
				{
					auto specular_intensity = pow(spec_value, mat->GetSpecPower());

					//Set Specular
					specular_color = mat->GetSpecularColour();
					specular_color.red *= specular_intensity;
					specular_color.green *= specular_intensity;
					specular_color.blue *= specular_intensity;
				}
			}

			//Set texture for the box
			Colour texture;
			texture.red = 1;
			texture.green = 1;
			texture.blue = 1;
			if (((Primitive*)hitresult->data)->m_primtype == Primitive::PRIMTYPE_Box)
			{
				auto box = ((Box*)hitresult->data);
				texture = box->GetTextureColour(surface_point);
			}

			//Calculate final colour
			outcolour.red += (diffuse_color.red * texture.red) + specular_color.red;
			outcolour.green += (diffuse_color.green * texture.green) + specular_color.green;
			outcolour.blue += (diffuse_color.blue * texture.blue) + specular_color.blue;

			lit_iter++;
		}
	}

	return outcolour;
}
