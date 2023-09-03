//object.h
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "material.h"

class Object
{
protected:
	Material *material;
	bool isSolid;
public:
	Object(Material *mat): material(mat) {}	
	virtual bool intersect(Ray& ray) const = 0;
	virtual Color shade(const Ray& ray) const
	{
		return material->shade(ray, isSolid);
	}
	// Sets the corresponding values to the variables passed by reference.
	virtual void setCoeff(double &ka, double &kd, double &ks, double &shiny) const
	{
		ka = this->material->ka;
		kd = this->material->kd;
		ks = this->material->ks;
		shiny = this->material->n;
	}
};

#endif
