#pragma once
#include"config.h"

class DRAM {
public:
	DRAM() :data(DRAMSize ,0){};

	vector<singleByte> data;
};