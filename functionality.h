#ifndef REMI_FUNCTIONALITY_H
#define REMI_FUNCTIONALITY_H

#include <cstdint>

template<typename T>
T foldl(T(*f)(T val, T val2), T iVal, T *buffer, uint32_t size)
{
	if (size == 0) return iVal;

	return f(buffer[0],
		foldl(f, iVal,
			buffer + 1,
			size - 1
			));
}

#endif //REMI_FUNCTIONALITY_H
