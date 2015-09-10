#pragma once

class InvBlock
{
public:
	int ID = 0;
	int amount = 0;

	InvBlock(int ID = 0, int amount = 0) : ID(ID), amount(amount) { }
};