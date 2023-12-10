#include <vector>

#include "Utils.h"
#include "../PlaneBSP.h"

bool Utils::CheckStringCoincidence(string stringA, string stringB)
{
	return stringA.find(stringB) != string::npos || stringB.find(stringA) != string::npos;
}

float Utils::FindMaxValue(vector<float> values)
{
	if (values.size() <= 0)
		return -1;

	float max = values[0];

	for (int i = 0; i < values.size(); i++)
	{
		if (values[i] > max)
		{
			max = values[i];
		}
	}

	return max;
}

float Utils::FindMinValue(vector<float> values)
{
	if (values.size() <= 0)
		return -1;

	float min = values[0];

	for (int i = 0; i < values.size(); i++)
	{
		if (values[i] < min)
		{
			min = values[i];
		}
	}

	return min;
}