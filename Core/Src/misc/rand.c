/*
 * rand.c
 *
 *  Created on: Mar 6, 2022
 *      Author: Gitmanik
 */

//https://stackoverflow.com/questions/52514296/simplest-random-number-generator-without-c-library

#include "rand.h"

static unsigned int g_seed;

// Used to seed the generator.
inline void fast_srand(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}
