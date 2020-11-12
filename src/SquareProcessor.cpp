#include "SquareProcessor.h"

void SquareProcessor::cycle()
{
	// 4. Exectuion Stage

	/********************************
	* Square Conveyor
	********************************/
	if (executionCountdown[0] == 0)
	{
		if (exec_inst[0].inst_type != InstructionType::NONE)
		{
			switch (exec_inst[0].inst_type)
			{
			case InstructionType::LDSBSC:
				sqc.load_from_sbu_to_in_buf(exec_inst[0]);
				if (exec_inst[0].dst_addr == 0)
				{
					in_buf_full_bit[0] = true;
				}
				else
				{
					in_buf_full_bit[1] = true;
				}
				break;
			case InstructionType::STSCSB:
				sqc.store_from_out_buf_to_sbu();
				if (exec_inst[0].dst_addr == 0)
				{
					out_buf_full_bit[0] = true;
				}
				else
				{
					out_buf_full_bit[1] = true;
				}
				break;
			default:
				break;
			}
			exec_inst[0].inst_type = InstructionType::NONE;
		}
	}
	else
	{
		executionCountdown[0]--;
	}

	/********************************
	* PE Array
	********************************/
	if (executionCountdown[1] == 0)
	{
		if (exec_inst[1].inst_type != InstructionType::NONE)
		{
			switch (exec_inst[1].inst_type)
			{
			case InstructionType::MAD:
				multiply_and_add();
				break;
			case InstructionType::IDV:
				iterative_divide();
				break;
			default:
				break;
			}
			exec_inst[1].inst_type = InstructionType::NONE;
		}
	}
	else
	{
		executionCountdown[1]--;
	}

	/********************************
	* RF Array
	********************************/
	if (executionCountdown[2] == 0)
	{
		if (exec_inst[2].inst_type != InstructionType::NONE)
		{
			switch (exec_inst[2].inst_type)
			{
			case InstructionType::LDSCRF:
				load_from_sqc_to_rfa();
				if (exec_inst[2].dst_reg_x_shiftable)
					rfa_reg_full_bit[exec_inst[2].dst_reg_x] = true;
				else
					pea_reg_full_bit[exec_inst[2].dst_reg_x] = true;
				break;
			case InstructionType::STRFSC:
				store_from_rfa_to_sqc();
				break;
			case InstructionType::SHIFT:
				rfa_shift();
				break;
			default:
				break;
			}
			exec_inst[2].inst_type = InstructionType::NONE;
		}
	}
	else
	{
		executionCountdown[2]--;
	}

	// 3. Issue Stage
	if (issueCountdown == 0)
	{
		bool issueSuccess = false;
		switch (issue_inst.inst_type)
		{
		case InstructionType::NONE:
			break;
		/********************************
		* Square Conveyor Issue Part
		********************************/
		case InstructionType::LDSBSC:
			if (sqc.check_LDSBSC(issue_inst))
			{
				exec_inst[0] = issue_inst;
				issueSuccess = true;
			}
			break;
		/********************************
		* PE Array Issue Part
		********************************/
		case InstructionType::MAD:
			if ((issue_inst.src_reg_a_shiftable&&rfa_reg_full_bit[issue_inst.src_reg_a]) || ((!issue_inst.src_reg_a_shiftable) && pea_reg_full_bit[issue_inst.src_reg_a]))
			{
				if ((issue_inst.src_reg_b_shiftable&&rfa_reg_full_bit[issue_inst.src_reg_b]) || ((!issue_inst.src_reg_b_shiftable) && pea_reg_full_bit[issue_inst.src_reg_b]))
				{
					exec_inst[1] = issue_inst;
					issueSuccess = true;
				}
			}
			break;
		case InstructionType::IDV:
			if ((issue_inst.src_reg_b_shiftable&&rfa_reg_full_bit[issue_inst.src_reg_b]) || ((!issue_inst.src_reg_b_shiftable) && pea_reg_full_bit[issue_inst.src_reg_b]))
			{
				if ((issue_inst.src_reg_c_shiftable&&rfa_reg_full_bit[issue_inst.src_reg_c]) || ((!issue_inst.src_reg_c_shiftable) && pea_reg_full_bit[issue_inst.src_reg_c]))
				{
					exec_inst[1] = issue_inst;
					issueSuccess = true;
				}
			}
			break;
		/********************************
		* RF Array Issue Part
		********************************/
		case InstructionType::LDSCRF:
			if (((exec_inst[2].src_addr == 0) && (in_buf_full_bit[0] == true)) || ((exec_inst[2].src_addr != 0) && (in_buf_full_bit[1] == true)))
			{
				if ((issue_inst.dst_reg_x_shiftable&&(!rfa_reg_full_bit[issue_inst.dst_reg_x])) || ((!issue_inst.dst_reg_x_shiftable) && (!pea_reg_full_bit[issue_inst.dst_reg_x])))
				{
					exec_inst[2] = issue_inst;
					issueSuccess = true;
				}
			}
			
		case InstructionType::STRFSC:
			break;
		case InstructionType::SHIFT:
			exec_inst[2] = issue_inst;
			issueSuccess = true;
			break;
		default:
			break;
		}
		if (issueSuccess)
		{
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

void SquareProcessor::load_from_sqc_to_rfa(const Instruction& inst)
{
	int sqc_bank_num = 0;

	int srcAddr = inst.src_addr;

	for (int i = 0; i < inst.square_size; i++)
	{
		for (int j = 0; j < inst.square_size; j++)
		{
			if (inst.dst_reg_x_shiftable)
			{
				rf_array[i][j].reg[inst.dst_reg_x] = sqc.in_buf.data[j][srcAddr];
			}
			else
			{
				pe_array[i][j].reg[inst.dst_reg_x] = sqc.in_buf.data[j][srcAddr];
			}
		}
		srcAddr++;
	}
}

void SquareProcessor::broadcast_from_weight_buf(const Instruction& inst)
{
	int sqc_bank_num = inst.bank_num;

	int srcAddr = inst.src_addr;

	
	if (inst.dst_reg_x_shiftable)
	{
		for (int i = 0; i < inst.square_size; i++)
		{
			for (int j = 0; j < inst.square_size; j++)
			{
				rf_array[i][j].reg[inst.dst_reg_x] = sqc.weight_buf.data[sqc_bank_num][srcAddr];
			}
		}
	}
	else
	{
		for (int i = 0; i < inst.square_size; i++)
		{
			for (int j = 0; j < inst.square_size; j++)
			{
				pe_array[i][j].reg[inst.dst_reg_x] = sqc.weight_buf.data[sqc_bank_num][srcAddr];
			}
		}
	}
		
}

void SquareProcessor::store_from_rfa_to_sqc(const Instruction& inst)
{

	int dstAddr = inst.dst_addr;
	if (inst.src_reg_a_shiftable)
	{
		for (int i = 0; i < inst.square_size; i++)
		{
			for (int j = 0; j < inst.square_size; j++)
			{
				sqc.out_buf.data[j][dstAddr] = rf_array[i][j].reg[inst.src_reg_a];
			}
		}
	}
	else
	{
		for (int i = 0; i < inst.square_size; i++)
		{
			for (int j = 0; j < inst.square_size; j++)
			{
				sqc.out_buf.data[j][dstAddr] = pe_array[i][j].reg[inst.src_reg_a];
			}
		}
	}
}

void SquareProcessor::multiply_and_add()
{
	for (int i = 0; i < PESizePerSTP; i++)
	{
		for (int j = 0; j < PESizePerSTP; j++)
		{
			int a = exec_inst[1].src_reg_a_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_a] : pe_array[i][j].reg[exec_inst[1].src_reg_a];
			int b = exec_inst[1].src_reg_b_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_b] : pe_array[i][j].reg[exec_inst[1].src_reg_b];
			int c = exec_inst[1].src_reg_c_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_c] : pe_array[i][j].reg[exec_inst[1].src_reg_c];
			int d = exec_inst[1].src_reg_d_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_d] : pe_array[i][j].reg[exec_inst[1].src_reg_d];
			int ans = a * b + (c << 16) + d;
			if (exec_inst[1].dst_reg_x_shiftable) 
				rf_array[i][j].reg[exec_inst[1].dst_reg_x] = ans >> 16;
			else 
				pe_array[i][j].reg[exec_inst[1].dst_reg_x] = ans >> 16;
			if (exec_inst[1].dst_reg_y_shiftable) 
				rf_array[i][j].reg[exec_inst[1].dst_reg_y] = ans;
			else 
				pe_array[i][j].reg[exec_inst[1].dst_reg_y] = ans;
		}
	}
}

void SquareProcessor::iterative_divide()
{
	for (int i = 0; i < PESizePerSTP; i++)
	{
		for (int j = 0; j < PESizePerSTP; j++)
		{
			// int a = exec_inst[1].src_reg_a_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_a] : pe_array[i][j].reg[exec_inst[1].src_reg_a];
			int b = exec_inst[1].src_reg_b_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_b] : pe_array[i][j].reg[exec_inst[1].src_reg_b];
			int c = exec_inst[1].src_reg_c_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_c] : pe_array[i][j].reg[exec_inst[1].src_reg_c];
			// int d = exec_inst[1].src_reg_d_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_d] : pe_array[i][j].reg[exec_inst[1].src_reg_d];
			int ans = b / c;
			if (exec_inst[1].dst_reg_y_shiftable)
				rf_array[i][j].reg[exec_inst[1].dst_reg_y] = ans;
			else
				pe_array[i][j].reg[exec_inst[1].dst_reg_y] = ans;
		}
	}
}

void SquareProcessor::rfa_shift()
{
	vector<doubleByte> temp_vector(exec_inst[1].bound, 0);
	switch (exec_inst[1].direction)
	{
	case Direction::left:
		for (int i = 0; i < exec_inst[1].bound; i++)
		{
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				temp_vector[(j + exec_inst[1].bound - exec_inst[1].square_size) % exec_inst[1].bound] = 
					exec_inst[1].src_reg_a_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_a] : pe_array[i][j].reg[exec_inst[1].src_reg_a];
			}
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				if (exec_inst[1].dst_reg_x_shiftable)
					rf_array[i][j].reg[exec_inst[1].dst_reg_x] = temp_vector[j];
				else
					pe_array[i][j].reg[exec_inst[1].dst_reg_y] = temp_vector[j];
			}
		}
		break;
	case Direction::right:
		for (int i = 0; i < exec_inst[1].bound; i++)
		{
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				temp_vector[(j + exec_inst[1].bound + exec_inst[1].square_size) % exec_inst[1].bound] = 
					exec_inst[1].src_reg_a_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_a] : pe_array[i][j].reg[exec_inst[1].src_reg_a];
			}
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				if (exec_inst[1].dst_reg_x_shiftable)
					rf_array[i][j].reg[exec_inst[1].dst_reg_x] = temp_vector[j];
				else
					pe_array[i][j].reg[exec_inst[1].dst_reg_y] = temp_vector[j];
			}
		}
		break;
	case Direction::up:
		for (int i = 0; i < exec_inst[1].bound; i++)
		{
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				temp_vector[(j + exec_inst[1].bound - exec_inst[1].square_size) % exec_inst[1].bound] = 
					exec_inst[1].src_reg_a_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_a] : pe_array[i][j].reg[exec_inst[1].src_reg_a];
			}
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				if (exec_inst[1].dst_reg_x_shiftable)
					rf_array[j][i].reg[exec_inst[1].dst_reg_x] = temp_vector[j];
				else
					pe_array[j][i].reg[exec_inst[1].dst_reg_y] = temp_vector[j];
			}
		}
		break;
	case Direction::down:
		for (int i = 0; i < exec_inst[1].bound; i++)
		{
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				temp_vector[(j + exec_inst[1].bound + exec_inst[1].square_size) % exec_inst[1].bound] = 
					exec_inst[1].src_reg_a_shiftable ? rf_array[i][j].reg[exec_inst[1].src_reg_a] : pe_array[i][j].reg[exec_inst[1].src_reg_a];
			}
			for (int j = 0; j < exec_inst[1].bound; j++)
			{
				if (exec_inst[1].dst_reg_x_shiftable)
					rf_array[j][i].reg[exec_inst[1].dst_reg_x] = temp_vector[j];
				else
					pe_array[j][i].reg[exec_inst[1].dst_reg_y] = temp_vector[j];
			}
		}
		break;
	default:
		break;
	}
}
