#pragma once
#include"config.h"
#include"Instruction.h"
#include"DRAM.h"
#include"SliceBuffer.h"

class ImageConveyor {
public:
	// explicit IOUnit() = default;
	ImageConveyor(DRAM& dram_instance, vector<SliceBuffer>& slice_buffer_pool_instance) :
		fetchCountdown(0), decodeCountdown(0), issueCountdown(0), executionCountdown(0), dram(dram_instance),sbp(slice_buffer_pool_instance){}

	void load_from_dram_to_sbu();
	void store_from_sbu_to_dram();
	bool check_load_from_sbu(const Instruction& inst);

	void cycle();

	// addrType dram_base;
	DRAM& dram;
	vector<SliceBuffer>& sbp;

	queue<Instruction> inst_buf;  // Instruction buffer
	Instruction decode_inst;
	Instruction issue_inst;
	Instruction exec_inst;

	int fetchCountdown;
	int decodeCountdown;
	int issueCountdown;
	int executionCountdown;
};