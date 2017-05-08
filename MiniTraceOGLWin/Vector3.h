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
class Vector3
{
private:
	double		m_element[3];

public:
	Vector3()
	{
		m_element[0] = 0.0f;
		m_element[1] = 0.0f;
		m_element[2] = 0.0f;
	}
	
	Vector3(double x, double y, double z)
	{
		m_element[0] = x; m_element[1] = y; m_element[2] = z;
	}
	
	virtual ~Vector3(){ ; }

	double operator [] (const int i) const;
	double& operator [] (const int i);
	Vector3 operator + (const Vector3& rhs) const;
	Vector3 operator - (const Vector3& rhs) const;
	Vector3& operator = (const Vector3& rhs);
	Vector3 operator * (const Vector3& rhs) const;
	Vector3 operator * (double scale) const;

	double Norm()	const;
	double Norm_Sqr() const;
	Vector3 Normalise();

	double DotProduct(const Vector3& rhs) const;
	Vector3 CrossProduct(const Vector3& rhs) const;
	Vector3 Reflect(const Vector3& n) const;
	Vector3 Refract(const Vector3& n, double r_coeff) const;
	
	void SetZero();
	
	inline void SetVector(double x, double y, double z)
	{ 
		m_element[0] = x; m_element[1] = y; m_element[2] = z; 
	}
};

