/*
 * libgcc/__clzdi2.c
 *
 * Returns the leading number of 0 bits in the argument
 */

#include <stdint.h>
#include <stddef.h>

uint64_t __clzdi2(uint64_t v)
{
	uint32_t vhi = (uint32_t)(v >> 32);
	uint32_t vx;
	int dp;

	if (vhi) {
		vx = vhi;
		dp = 32;
	} else {
		vx = (uint32_t)v;
		dp = 0;
	}

	return dp + __builtin_clz(vx);
}
