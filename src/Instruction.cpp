#include "Instruction.h"

string Instruction::print_type()
{
	switch (inst_type)
	{
	case InstructionType::NONE:
		return "NONE";
	case InstructionType::LDDRSB:
		return "LDDRSB";
	case InstructionType::STSBDR:
		return "STSBDR";
	case InstructionType::LDSBSC:
		return "LDSBSC";
	case InstructionType::LDSBW:
		return "LDSBW";
	case InstructionType::STSCSB:
		return "STSCSB";
	case InstructionType::LDSCRF:
		return "LDSCRF";
	case InstructionType::LDWRF:
		return "LDWRF";
	case InstructionType::STRFSC:
		return "STRFSC";
	case InstructionType::MAD:
		return "MAD";
	case InstructionType::IDV:
		return "IDV";
	case InstructionType::SHIFT:
		return "SHIFT";
	default:
		return "ERROR";
	}
}
