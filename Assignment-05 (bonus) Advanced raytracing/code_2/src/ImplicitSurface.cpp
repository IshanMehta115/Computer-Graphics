//ImplicitSurface.cpp
#include "ImplicitSurface.h"
#include "world.h"


// returns the implicit function value at point P.
float ImplicitSurface::value(const Vector3D &p) const
{
	double x = p[0];
	double y = p[1];
	double z = p[2];

	double x_2 = x*x;
	double y_2 = y*y;
	double z_2 = z*z;

	// double term1 = 2 * y * (y_2 - 3*x_2)*(1 - z_2);
	// double term2 = pow(x_2 + y_2,2);
	// double term3 = (9*z_2 - 1) * (1-z_2);
	// return (float)(term1  + term2 - term3);

	return (2 * y * (y_2 - 3*x_2) - (9*z_2 - 1)) * (1-z_2) + (x_2 + y_2)*(x_2 + y_2);
}
// returns the normal at point of p on the surface. Using partial derivatives
Vector3D ImplicitSurface::GetNormal(const Vector3D &p) const
{
	double x = p[0];
	double y = p[1];
	double z = p[2];
	double dx = 2*y*(1-z*z)*(-6*x) + 4*x*(x*x + y*y);
	double dy = (1-z*z)*(2*(y*y - 3*x*x) + 4*y*y) + 4*y*(x*x + y*y);
	double dz = 2*y*(y*y - 3*x*x)*(-2*z) - (18*z*(1-z*z) + (9*z*z-1)*(-2*z));
	Vector3D normal(dx,dy,dz);
	normal.normalize();
	return normal;
}
// Uses Ray marching algorithm to test whether Ray r intersects with the object give by implicit equation.
bool ImplicitSurface::intersect(Ray& r) const
{
	Vector3D O = r.getOrigin();
	Vector3D D = r.getDirection();
	double dt = 0.1;	// delta value for t.
	double t = 0;		// initial value of t.
	double t_max = 50;	// max value of t.
	while(t < t_max && value(O + t*D) > 0.1)
		t+=dt;
	if(t<t_max)
	{
		r.setParameter(t, this, this->GetNormal(O + t*D));
		return true;
	}
	return false;
}