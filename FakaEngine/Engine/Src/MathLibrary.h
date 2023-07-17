#ifndef MYMATHLIBRARY_H
#define MYMATHLIBRARY_H

#include "PrivateClass/Export.h"

class ENGINE_API MathLibrary 
{
public:
	MathLibrary();
	~MathLibrary();

	//Metodo que calcula las normales de cada vertice segun los datos de posicion de estos.
	void CalcAverageNormals(unsigned int* indices,
		unsigned int indicesCount,
		float* vertices,
		unsigned int verticesCount,
		unsigned int vLenght,
		unsigned int normalOffset);

	float abs(float num);
	float acos(float num);
	float acosh(float num);
	float asin(float num);
	float asinh(float num);
	float atan(float num);
	float atanh(float num);
	int bitCount(int num);
	double ceil(double num);
	double clamp(double num, double min, double max);
	float cos(float num);
	float cosh(float num);
	float distance(float num);
	float ToRadians(float num);
	float sin(float num);
	float tan(float num);
};

#endif