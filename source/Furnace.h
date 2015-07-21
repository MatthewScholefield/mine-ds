#pragma once
#include "InvBlock.h"

class Furnace
{
public:
	InvBlock source, fuel, result;

	Furnace() : source(), fuel(), result() { }

	Furnace(InvBlock source, InvBlock fuel) : source(source), fuel(fuel), result() { }
};