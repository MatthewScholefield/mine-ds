#pragma once
#include "InvBlock.h"

class Furnace
{
public:
	InvBlock source, fuel, result;

	Furnace()
	{
		source = fuel = result = InvBlock();
	}

	Furnace(InvBlock source, InvBlock fuel) : source(source), fuel(fuel)
	{
		result = InvBlock();
	}
};