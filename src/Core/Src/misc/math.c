/*
 * math.c
 *
 *  Created on: 5 mar 2022
 *      Author: Gitmanik
 */

#include "math.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void ftoa(float num, char* buf)
{
	assert(num > 0);

	int przed = (int) num;
	int po = (int) ((num - przed) * 100);
	utoa(przed, buf, 10);
	int len = strlen(buf);
	buf[len] = '.';
	char buf2[2];
	utoa(po, buf2, 10);
	buf[len+1] = buf2[0];
	buf[len+2] = buf2[1];
	buf[len+3] = '\0';
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
