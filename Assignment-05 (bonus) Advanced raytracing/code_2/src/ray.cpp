#include "ray.h"

bool Ray::setParameter(const float par, const Object *obj, const Vector3D norm)
{
	if(par < t && par > SMALLEST_DIST)
	{
		hit = true;
		t = par;
		object = obj;
		this->normal = norm;		// sets the normal of object
		return true;
	}
	return false;
}
