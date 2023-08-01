#include "Utils.h"

bool Utils::CheckStringCoincidence(string stringA, string stringB)
{
	return stringA.find(stringB) != string::npos || stringB.find(stringA) != string::npos;
}