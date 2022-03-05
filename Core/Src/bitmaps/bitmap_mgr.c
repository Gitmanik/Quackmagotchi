#include "anim_quack.h"

int* ANIM_Quack[4];

void ANIM_Init()
{
	  ANIM_Quack[0] = &quack0;
	  ANIM_Quack[1] = &quack1;
	  ANIM_Quack[2] = &quack2;
	  ANIM_Quack[3] = &quack0;
}
