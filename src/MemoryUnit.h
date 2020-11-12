#pragma once
#include"config.h"

class MemoryUnit {
public:
	explicit MemoryUnit() = default;
	MemoryUnit(int bankSize, int bankNum) :
		bank_size(bankSize),
		bank_num(bankNum),
		data(bankNum, vector<singleByte>(bankSize, 0)) {};

	vector<vector<singleByte>> data;  // data[x][y] means a 16bit data in bank 'x', location at 'y'

	int bank_size;  // The number of dataType per bank.
	int bank_num;  // The number of banks. 
};