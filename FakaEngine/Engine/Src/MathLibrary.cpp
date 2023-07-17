#include "MathLibrary.h"
#include "../../glm/glm.hpp"
#include "../Lib/glm/vec4.hpp"
#include "../Lib/glm/vec3.hpp"
#include "../Lib/glm/vec2.hpp"
#include "../Lib/glm/mat4x4.hpp"
#include "../Lib/glm/mat3x3.hpp"
#include "../Lib/glm/mat2x2.hpp"

MathLibrary::MathLibrary(){}

MathLibrary::~MathLibrary(){}

void MathLibrary::CalcAverageNormals(unsigned int * indices, unsigned int indiceCount, 
	float * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
	for (int i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (int i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

float MathLibrary::ToRadians(float num)
{
	return glm::radians(num);
}

float MathLibrary::sin(float num)
{
	return glm::sin(num);
}

float MathLibrary::cos(float num)
{
	return glm::cos(num);
}
float MathLibrary::cosh(float num)
{
	return glm::cosh(num);
}

float MathLibrary::distance(float num)
{
	return distance(num);
}

float MathLibrary::tan(float num)
{
	return glm::tan(num);
}

float MathLibrary::abs(float num)
{
	return glm::abs(num);
}

float MathLibrary::acos(float num)
{
	return glm::acos(num);
}

float MathLibrary::acosh(float num)
{
	return acosh(num);
}

float MathLibrary::asin(float num)
{
	return glm::asin(num);
}

float MathLibrary::asinh(float num)
{
	return asinh(num);
}

float MathLibrary::atan(float num)
{
	return glm::atan(num);
}

float MathLibrary::atanh(float num)
{
	return glm::atanh(num);
}

int MathLibrary::bitCount(int num)
{
	return glm::bitCount(num);
}

double MathLibrary::ceil(double num)
{
	return glm::ceil(num);
}

double MathLibrary::clamp(double num, double min, double max)
{
	return glm::clamp(num, min, max);
}
