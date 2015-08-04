

#ifndef PTZNEON_H
#define PTZNEON_H

#include "PTZMinking.h"

class CPTZNeon : public CPTZMinking  
{
public:
	CPTZNeon();
	virtual ~CPTZNeon();
};

class CPTZStar : public CPTZMinking  
{
public:
	CPTZStar();
	virtual ~CPTZStar();
};

class CPTZVido : public CPTZMinking  
{
public:
	CPTZVido();
	virtual ~CPTZVido();
};

#endif
