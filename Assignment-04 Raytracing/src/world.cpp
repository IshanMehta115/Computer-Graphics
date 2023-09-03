#include "world.h"
#include<iostream>
using namespace std;

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++)
		objectList[i]->intersect(ray);
	return ray.getParameter();
}

Color World::shade_ray(Ray& ray)
{
	firstIntersection(ray);
	if(ray.didHit())
	{
		double ka,kd,ks,shiny;
		(ray.intersected())->setCoeff(ka,kd,ks,shiny);
		Vector3D norm = ray.getNormal();
		Color c = ambient * ka;				// add base ambient light.
		for(LightSource* ls:lightSourceList)	// traverse over all light sources
		{

			// shadow ray starts from point on the object to the light source
			Ray shadow_ray(ray.getPosition(), ls->getPosition() - ray.getPosition());
			firstIntersection(shadow_ray);

			// if shadow ray doesnt hit any objectt then this means the path is not obstructed.
			if(!shadow_ray.didHit())
			{
				Vector3D h = (ls->getPosition() - ray.getPosition()) + (Vector3D(0,0,10) - ray.getPosition());
				h.normalize();
				Vector3D light_dir = ls->getPosition() - ray.getPosition();
				light_dir.normalize();

				// diffuse light comp
				Color diff_light = ls->getIntensity() * (kd * max(0.0, dotProduct(light_dir, norm)));
				
				// specular light comp
				Color spec_light = ls->getIntensity() * (ks * pow(max(0.0, dotProduct(h, norm)),shiny));
				c = c + diff_light + spec_light;
			}			
		}
		return (ray.intersected())->shade(ray) * c;
		
	}
	return background;
}
