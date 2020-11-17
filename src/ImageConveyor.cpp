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
	sbp[exec_inst.sbu_idx].write_ptr = dstAddr+1;
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

bool ImageConveyor::check_load_from_sbu(const Instruction & inst)
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

void ImageConveyor::cycle()
{
	cout << "Image Conveyor:" << endl;
	cout << "Decode Inst.: " << decode_inst.print_type() << endl;
	cout << "Issue Inst.: " << issue_inst.print_type() << endl;
	cout << "Execute Inst.: " << exec_inst.print_type() << endl;

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
			switch (issue_inst.inst_type)
			{
			case InstructionType::LDDRSB:
				exec_inst = issue_inst;
				executionCountdown = issue_inst.exec_time;
				issue_inst.inst_type = InstructionType::NONE;
				issueCountdown = ImageConveyorIssueDelay;
				break;
			case InstructionType::STSBDR:
				if (check_load_from_sbu(issue_inst))
				{
					exec_inst = issue_inst;
					issue_inst.inst_type = InstructionType::NONE;
					issueCountdown = ImageConveyorIssueDelay;
				}
				break;
			default:
				break;
			}
			
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
			decode_inst.inst_type = InstructionType::NONE;
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
		if ((decode_inst.inst_type == InstructionType::NONE) && (!inst_buf.empty()))
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
