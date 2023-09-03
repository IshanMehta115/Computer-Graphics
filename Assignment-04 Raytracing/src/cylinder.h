//sphere.h
#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class Cylinder : public Object
{
private:
	Vector3D cen1, cen2;
	double radius;

public:
	Triangle(const Vector3D& _cen1,const Vector3D& _cen2,const double& _r, Material* mat):
		Object(mat), cen1(_cen1), cen2(_cen2), radius(_r);
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
	virtual Vector3D GetNormal() const;
};
#endif
