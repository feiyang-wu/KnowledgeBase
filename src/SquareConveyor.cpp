#include "SquareConveyor.h"

void SquareConveyor::load_from_sbu_to_in_buf(const Instruction& inst)
{
	int sbu_bank_num = 0;
	int sqc_bank_num = inst.bank_num;
	int size_square = inst.square_size * inst.square_size;
	int srcAddr = inst.src_addr;
	int dstAddr = inst.dst_addr;
	for (int i = 0; i < size_square; i++)
	{
		in_buf.data[(sqc_bank_num) % inst.square_size][dstAddr] = sbp[inst.sbu_idx].data[sbu_bank_num][srcAddr];
		sqc_bank_num++;
		sbu_bank_num++;
		if (sqc_bank_num >= inst.square_size)
		{
			sqc_bank_num = 0;
			dstAddr++;
		}
		if (sbu_bank_num >= sbp[inst.sbu_idx].bank_num)
		{
			sbu_bank_num = 0;
			srcAddr++;
		}
	}
}

void SquareConveyor::load_from_sbu_to_weight_buf(const Instruction& inst)
{
	int sbu_bank_num = 0;
	int sqc_bank_num = inst.bank_num;

	int srcAddr = inst.src_addr;
	int dstAddr = inst.dst_addr;
	for (int i = 0; i < inst.ldst_data_size; i++)
	{
		weight_buf.data[(sqc_bank_num) % WeightBufferBankNum][dstAddr] = sbp[inst.sbu_idx].data[sbu_bank_num][srcAddr];
		sqc_bank_num++;
		sbu_bank_num++;
		if (sqc_bank_num >= WeightBufferBankNum)
		{
			sqc_bank_num = 0;
			dstAddr++;
		}
		if (sbu_bank_num >= sbp[inst.sbu_idx].bank_num)
		{
			sbu_bank_num = 0;
			srcAddr++;
		}
	}
}

void SquareConveyor::store_from_out_buf_to_sbu(const Instruction& inst)
{
	int sbu_bank_num = inst.bank_num;
	int sqc_bank_num = 0;
	int size_square = inst.square_size * inst.square_size;
	int srcAddr = inst.src_addr;
	int dstAddr = inst.dst_addr;
	for (int i = 0; i < size_square; i++)
	{
		sbp[inst.sbu_idx].data[sbu_bank_num][dstAddr] = out_buf.data[(sqc_bank_num) % inst.square_size][srcAddr];
		sqc_bank_num++;
		sbu_bank_num++;
		if (sqc_bank_num >= inst.square_size)
		{
			sqc_bank_num = 0;
			srcAddr++;
		}
		if (sbu_bank_num >= sbp[inst.sbu_idx].bank_num)
		{
			sbu_bank_num = 0;
			dstAddr++;
		}
	}
	sbp[inst.sbu_idx].write_ptr = dstAddr+1;
}

bool SquareConveyor::check_load_from_sbu(const Instruction & inst)
{
	int readEnd = inst.src_addr + inst.square_size * inst.square_size / SliceBufferBankNum;
	if (readEnd <= sbp[inst.sbu_idx].write_ptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
