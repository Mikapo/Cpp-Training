#include "MikaLibrary.h"
#include <cmath>

double mikalib::vector::size() const
{
	return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
}

void mikalib::vector::normalize()
{
	double current_size = size();
	if (current_size == 0) return;

	*this = *this / current_size;
}


