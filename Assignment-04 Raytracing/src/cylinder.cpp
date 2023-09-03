//triangle.cpp
#include "imgui_setup.h"
#include "triangle.h"
using namespace std;


Vector3D Triangle::GetNormal() const
{
	Vector3D v1 = this->pos2 - this->pos1;
	Vector3D v2 = this->pos3 - this->pos1;
	Vector3D norm = crossProduct(v1,v2);
	norm.normalize();
	return norm;
}
bool Triangle::intersect(Ray& r) const
{
	glm::mat3 A;
	for(int i=0;i<3;i++) A[i][0] = this->pos1[i] - this->pos2[i];
	for(int i=0;i<3;i++) A[i][1] = this->pos1[i] - this->pos3[i];
	for(int i=0;i<3;i++) A[i][2] = r.getDirection()[i];

	glm::mat3 beta_matrix = A,gamma_matrix = A, par_matrix = A;

	for(int i=0;i<3;i++) beta_matrix[i][0] = this->pos1[i] - r.getOrigin()[i];
	for(int i=0;i<3;i++) gamma_matrix[i][1] = this->pos1[i] - r.getOrigin()[i];
	for(int i=0;i<3;i++) par_matrix[i][2] = this->pos1[i] - r.getOrigin()[i];


	float DET = glm::determinant(A);

	if(0==DET) return false;


	float beta_value = glm::determinant(beta_matrix)/DET;
	float gamme_value = glm::determinant(gamma_matrix)/DET;
	float par_value = glm::determinant(par_matrix)/DET;

	if(beta_value > 0 && gamme_value > 0 && (beta_value + gamme_value) < 1){
		r.setParameter(par_value, this, this->GetNormal());
		return true;
	}
	return false;

}
