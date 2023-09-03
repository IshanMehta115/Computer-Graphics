//sphere.cpp

#include "sphere.h"
#include "world.h"
#include "iostream"
using namespace std;


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
			r.setParameter(t, this, this->GetNormal(temp));
			return true;
		}
		else
		{
			//Calculate both roots
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);


			temp = r.getOrigin() + t1*r.getDirection();
			bool b1 = r.setParameter(t1, this, this->GetNormal(temp));

			temp = r.getOrigin() + t2*r.getDirection();
			bool b2 = r.setParameter(t2, this, this->GetNormal(temp));

			return b1||b2;    
		}
	}
	return false;
}