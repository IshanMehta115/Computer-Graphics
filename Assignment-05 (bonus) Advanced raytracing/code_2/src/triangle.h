//sphere.h
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class Triangle : public Object
{
private:
	Vector3D pos1, pos2, pos3;

public:
	Triangle(const Vector3D& _pos1,const Vector3D& _pos2,const Vector3D& _pos3, Material* mat):
		Object(mat), pos1(_pos1), pos2(_pos2), pos3(_pos3)
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
	virtual Vector3D GetNormal() const;
};
#endif
