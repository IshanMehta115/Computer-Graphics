#include "ray.h"

bool Ray::setParameter(const float par, const Object *obj, const Vector3D norm, const double u, const double v)
{
	if(par < t && par > SMALLEST_DIST)
	{
		hit = true;
		t = par;
		object = obj;
		this->normal = norm;		// sets the normal of object
		this->texture_u = u;		// sets the texture coordinate u
		this->texture_v = v;		// sets the texture coordinate v
		return true;
	}
	return false;
}
