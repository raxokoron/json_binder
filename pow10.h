#pragma once

template<typename T>
T pow10(unsigned int pow)
{
	T r = 1;
	T p = 10;
	while (pow != 0) 
	{
		if (pow & 1) 
			r *= p;

		p *= p;
		pow >>= 1;
	}

	return r;
}