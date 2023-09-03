//sphere.cpp

#include "sphere.h"
#include "world.h"


// maps the point p on sphere to 2d texture space (u,v) and return u
double Sphere::get_texture_coordinate_u(const Vector3D &p) const
{
	double X = p[0] - this->position[0];
	double Y = p[1] - this->position[1];
	double Z = p[2] - this->position[2];
	double u = (atan2(X,Z) + (3.1415)) / (2 * ((3.1415)));
	return u;
}

// maps the point p on sphere to 2d texture space (u,v) and return v
double Sphere::get_texture_coordinate_v(const Vector3D &p) const
{
	double X = p[0] - this->position[0];
	double Y = p[1] - this->position[1];
	double Z = p[2] - this->position[2];
	double v = acos(Y/radius) / (3.1415);
	return v;
}

// returns the normal at point of v on the sphere
Vector3D Sphere::GetNormal(const Vector3D &v) const
{
	Vector3D temp = v - this->position;
	temp.normalize();
	// temp = -temp;
	return temp;
}
bool Sphere::intersect(Ray& r) const
{
	Vector3D centerVector = r.getOrigin() - position;
	double a = 1.0;
	double b = 2*dotProduct(r.getDirection(), centerVector);
	double c = dotProduct(centerVector, centerVector) - radius*radius;
	double discriminant = b*b - 4.0*a*c;

	//now check if discr. is posivtive or zero, then only we have an intersection!
	Vector3D temp;
	if(discriminant >=0.0)
	{

		if(discriminant == 0)
		{
			double t;
			t = -b/(2.0*a);
			temp = r.getOrigin() + t*r.getDirection();
			r.setParameter(t, this, this->GetNormal(temp), this->get_texture_coordinate_u(temp),this->get_texture_coordinate_v(temp));
			return true;
		}
		else
		{
			//Calculate both roots
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);


			temp = r.getOrigin() + t1*r.getDirection();
			bool b1 = r.setParameter(t1, this, this->GetNormal(temp), this->get_texture_coordinate_u(temp),this->get_texture_coordinate_v(temp));

			temp = r.getOrigin() + t2*r.getDirection();
			bool b2 = r.setParameter(t2, this, this->GetNormal(temp), this->get_texture_coordinate_u(temp),this->get_texture_coordinate_v(temp));

			return b1||b2;    
		}
	}
	return false;
}