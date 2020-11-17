#pragma once
#include"config.h"

class SliceBuffer {
public:
	SliceBuffer(int Idx) :
		bank_num(SliceBufferBankNum),
		bank_size(SliceBufferBankSize),
		data(SliceBufferBankNum, vector<singleByte>(SliceBufferBankSize, 0)),
		idx(Idx), 
		write_ptr(-1),
		varID(0),
		square_size(0),
		width_size(0),
		height_size(0)
	{};

	vector<vector<singleByte>> data;  // data[x][y] means a 16bit data in bank 'x', location at 'y'

	int bank_size;  // The number of dataType per bank.
	int bank_num;  // The number of banks. 

	addrType write_ptr;
	vector<addrType> read_ptr;

	int idx;
	int varID;  // Variable ID in this LBU
	int square_size;  // Variable in this LBU's size

	int width_size; // width of load image's size or sth
	int height_size; // height of load image's size or sth
};