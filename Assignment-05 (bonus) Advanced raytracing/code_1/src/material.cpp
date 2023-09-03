#include "world.h"
#include "material.h"

Color Material::shade(const Ray& incident, const bool isSolid) const
{
	double u = incident.get_object_texture_u();
	double v = incident.get_object_texture_v();
	u*=texture_width * 3;
	v*=texture_height * 3;
	int ii = u;
	int jj = v;
	ii%=texture_width;
	jj%=texture_height;
	double r = *(texture_data + (ii*texture_width + jj)*nrChannels + 0);
	double g = *(texture_data + (ii*texture_width + jj)*nrChannels + 1);
	double b = *(texture_data + (ii*texture_width + jj)*nrChannels + 2);
	// r = 0.1;
	// g = 0;
	// b = 0;
	Color c(r,g,b);
	Vector3D temp(c.R(), c.G(), c.B());
	temp.normalize();
	c.R(temp[0]);
	c.G(temp[1]);
	c.B(temp[2]);
	return c;
}
// functions to set properties of texture in material object.
void Material::set_texture(const unsigned char* data)
{
	this->texture_data = data;
}
void Material::set_texture_width(const int width)
{
	this->texture_width = width;
}
void Material::set_texture_height(const int height)
{
	this->texture_height = height;
}
void Material::set_texture_nrChannels(const int nr)
{
	this->nrChannels = nr;
}