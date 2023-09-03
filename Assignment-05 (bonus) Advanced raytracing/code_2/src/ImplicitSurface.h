//ImplicitSurface.h
#ifndef _ImplicitSurface_H_
#define _ImplicitSurface_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class ImplicitSurface : public Object
{
public:
	ImplicitSurface(Material* mat):
		Object(mat)
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
	// virtual Color shade(const Ray& ray) const;
	virtual Vector3D GetNormal(const Vector3D &v) const;
	virtual float value(const Vector3D &p) const;
};
#endif
