#pragma once

class InvBlock
{
public:
	int blockId = 0;
	int blockAmount = 0;

	InvBlock(int blockId = 0, int blockAmount = 0) : blockId(blockId), blockAmount(blockAmount) { }
};