#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

#define RAND_INV_RANGE(r)  ((int) (RAND_MAX / (r)))

uint32_t get_random_number(uint32_t range);

#endif /** _COMMON_H_**/
