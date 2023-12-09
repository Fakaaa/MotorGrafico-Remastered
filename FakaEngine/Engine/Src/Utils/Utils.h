#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

#include "../PlaneBSP.h"

using namespace std;

static class Utils
{
public:

	bool static CheckStringCoincidence(string stringA, string stringB);
	float static FindMaxValue(vector<float> values);
	float static FindMinValue(vector<float> values);
};
#endif // !UTILS_H