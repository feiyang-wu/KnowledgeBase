#pragma once
#include"config.h"
#include"Instruction.h"
#include"MemoryUnit.h"
#include"SliceBuffer.h"

class SquareConveyor {
public:
	SquareConveyor(vector<SliceBuffer>& sliceBufferPool, int Idx) :
		sbp(sliceBufferPool),
		idx(Idx),
		in_buf(InBufferBankSize, InBufferBankNum),
		out_buf(OutBufferBankSize, OutBufferBankNum),
		weight_buf(WeightBufferBankSize, WeightBufferBankNum) {};

	void load_from_sbu_to_in_buf(const Instruction& inst);
	void load_from_sbu_to_weight_buf(const Instruction& inst);
	void store_from_out_buf_to_sbu(const Instruction& inst);

	bool check_load_from_sbu(const Instruction& inst);

	vector<SliceBuffer>& sbp;

	int idx;
	MemoryUnit in_buf;
	MemoryUnit out_buf;
	MemoryUnit weight_buf;
};