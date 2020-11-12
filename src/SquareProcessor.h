#pragma once
#include"config.h"
#include"Instruction.h"
#include"SquareConveyor.h"

class RegisterFile {
public:
	RegisterFile() = default;
	RegisterFile(int rowIdx, int colIdx) :row_idx(rowIdx), col_idx(colIdx), reg(RegSizePerRF, 0) {};
	//void write(int des_reg_idx, dataType num);
	//dataType read(int des_reg_idx);

	int row_idx;
	int col_idx;

	vector<doubleByte> reg;
};

class ProcessingElement {
public:
	ProcessingElement() = default;
	ProcessingElement(int rowIdx, int colIdx) :row_idx(rowIdx), col_idx(colIdx), reg(RegSizePerPE, 0) {};

	int row_idx;
	int col_idx;

	vector<doubleByte> reg;
};

class SquareProcessor {
public:
	SquareProcessor(SquareConveyor& squareConveyor, int Idx) :sqc(squareConveyor),idx(Idx)
	{
		for (int i = 0; i < PESizePerSTP; i++)
		{
			vector<ProcessingElement> tmp;
			for (int j = 0; j < PESizePerSTP; j++)
			{
				ProcessingElement tmppe(i, j);
				tmp.push_back(tmppe);
			}
			pe_array.push_back(tmp);
		}

		for (int i = 0; i < RFSizePerSTP; i++)
		{
			vector<RegisterFile> tmp;
			for (int j = 0; j < RFSizePerSTP; j++)
			{
				RegisterFile tmprf(i, j);
				tmp.push_back(tmprf);
			}
			rf_array.push_back(tmp);
		}
	};

	void cycle();

	void load_from_sqc_to_rfa(const Instruction& inst);
	void broadcast_from_weight_buf(const Instruction& inst);
	void store_from_rfa_to_sqc(const Instruction& inst);

	void multiply_and_add();  // [X, Y] = A * B + [C, D]
	void iterative_divide();  // [X, Y] = [A, B] / C, A = 0x0000, ans at Y
	void rfa_shift();

	int idx;

	vector<bool> in_buf_full_bit;
	vector<bool> out_buf_full_bit;
	vector<bool> weight_buf_full_bit;
	vector<bool> pea_reg_full_bit;
	vector<bool> rfa_reg_full_bit;

	SquareConveyor& sqc;
	

	vector<vector<ProcessingElement>> pe_array;
	vector<vector<RegisterFile>> rf_array;

	vector<vector<doubleByte>> line_shared_ram;

	queue<Instruction> inst_buf;  // Inst. buffer
	Instruction decode_inst;
	Instruction issue_inst;
	vector<Instruction> exec_inst;  // [0: Square Conveyor] [1: PE Array] [2: Register File Array]

	int fetchCountdown;
	int decodeCountdown;
	int issueCountdown;
	vector<int> executionCountdown;  // [0: Square Conveyor] [1: PE Array] [2: Register File Array]
};