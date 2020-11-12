#include "ImageConveyor.h"

void ImageConveyor::load_from_dram_to_sbu()
{
	addrType dstAddr = sbp[exec_inst.sbu_idx].write_ptr + 1;
	int bankNum = exec_inst.bank_num;
	for (int i = 0; i < exec_inst.width_size; i++)
	{
		addrType srcAddr = exec_inst.src_addr + (exec_inst.start_x + i) * exec_inst.width_img + exec_inst.start_y;
		
		for (int j = 0; j < exec_inst.height_size; j++)
		{
			sbp[exec_inst.sbu_idx].data[bankNum][dstAddr] = dram.data[srcAddr + j];
			bankNum++;
			if (bankNum >= sbp[exec_inst.sbu_idx].bank_num)
			{
				dstAddr++;
				bankNum = 0;
			}
		}

	}
	sbp[exec_inst.sbu_idx].write_ptr = dstAddr;
}

void ImageConveyor::store_from_sbu_to_dram()
{
	addrType srcAddr = exec_inst.src_addr;
	int bankNum = exec_inst.bank_num;
	for (int i = 0; i < exec_inst.width_size; i++)
	{
		addrType dstAddr = exec_inst.dst_addr + (exec_inst.start_x + i) * exec_inst.width_img + exec_inst.start_y;

		for (int j = 0; j < exec_inst.height_size; j++)
		{
			dram.data[dstAddr + j] = sbp[exec_inst.sbu_idx].data[bankNum][srcAddr];
			bankNum++;
			if (bankNum >= sbp[exec_inst.sbu_idx].bank_num)
			{
				srcAddr++;
				bankNum = 0;
			}
		}

	}
}

void ImageConveyor::cycle()
{
	// 4. Exectuion Stage
	if (executionCountdown == 0)
	{
		if (exec_inst.inst_type != InstructionType::NONE)
		{
			switch (exec_inst.inst_type)
			{
			case InstructionType::LDDRSB:
				load_from_dram_to_sbu();
				break;
			case InstructionType::STSBDR:
				store_from_sbu_to_dram();
				break;
			default:
				break;
			}
			exec_inst.inst_type = InstructionType::NONE;
		}
	}
	else
	{
		executionCountdown--;
	}

	// 3. Issue Stage
	if (issueCountdown == 0)
	{
		if ((issue_inst.inst_type != InstructionType::NONE) && (exec_inst.inst_type == InstructionType::NONE))
		{
			exec_inst = issue_inst;
			issueCountdown = ImageConveyorIssueDelay;
		}
	}
	else
	{
		issueCountdown--;
	}

	// 2. Decode Stage
	if (decodeCountdown == 0)
	{
		if ((decode_inst.inst_type != InstructionType::NONE) && (issue_inst.inst_type == InstructionType::NONE))
		{
			issue_inst = decode_inst;
			decodeCountdown = ImageConveyorDecodeDelay;
		}
	}
	else
	{
		decodeCountdown--;
	}

	// 1. Fetch Stage
	if (fetchCountdown == 0)
	{
		if (decode_inst.inst_type == InstructionType::NONE)
		{
			decode_inst = inst_buf.front();
			inst_buf.pop();
			fetchCountdown = ImageConveyorDecodeDelay;
		}
	}
	else
	{
		fetchCountdown--;
	}
}
