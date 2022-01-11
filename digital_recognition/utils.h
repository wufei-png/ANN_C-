

#ifndef __UTILS__
#define __UTILS__

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>

using namespace std;


//0-1
inline double RandFloat()
{
	return (rand()) / (RAND_MAX + 1.0);
}


inline double RandomFloat0_5()//
{      //cout<<(rand()) / (RAND_MAX + 1.0)-0.5<<endl;
	return (rand()) / (RAND_MAX + 1.0)- 0.5;
}

#endif

