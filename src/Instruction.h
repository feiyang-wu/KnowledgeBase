#pragma once
#include"config.h"

class Instruction {
public:
	Instruction():
		inst_type(InstructionType::NONE),
		src_addr(0),
		dst_addr(0),
		sbu_idx(0),
		sqc_idx(0),
		width_img(0),
		height_img(0),
		width_size(0),
		height_size(0),
		start_x(0),
		start_y(0),
		bank_num(0),
		square_size(0),
		src_reg_a_shiftable(false),
		src_reg_a(0),
		src_reg_b_shiftable(false),
		src_reg_b(0),
		src_reg_c_shiftable(false),
		src_reg_c(0),
		src_reg_d_shiftable(false),
		src_reg_d(0),
		dst_reg_x_shiftable(false),
		dst_reg_x(0),
		dst_reg_y_shiftable(false),
		dst_reg_y(0),
		direction(Direction::NONE),
		bound(20),
		exec_time(0)
	{};            

	string print_type();

	InstructionType inst_type;

	// Load & Store
	addrType src_addr;
	addrType dst_addr;
	int ldst_data_size;

	int sbu_idx; // Slice Buffer Index
	int sqc_idx; // Square Conveyor Index

	int width_img; // width of image or sth
	int height_img; // height of image or sth
	int width_size; // width of load image's size or sth
	int height_size; // height of load image's size or sth
	int start_x; // Start point's x(width) of load image or sth
	int start_y; //  Start point's x(height) of load image or sth
	int bank_num; // load/store sth into SRAM start from a special bank number
	int square_size; // Size of the square, usually 16/18/20

	// Calculate & RegShift
	bool src_reg_a_shiftable; // choose shift or non-shift RegFile
	int src_reg_a;
	bool src_reg_b_shiftable; // choose shift or non-shift RegFile
	int src_reg_b;
	bool src_reg_c_shiftable; // choose shift or non-shift RegFile
	int src_reg_c;
	bool src_reg_d_shiftable; // choose shift or non-shift RegFile
	int src_reg_d;

	bool dst_reg_x_shiftable; // choose shift or non-shift RegFile
	int dst_reg_x;
	bool dst_reg_y_shiftable; // choose shift or non-shift RegFile
	int dst_reg_y;

	Direction direction; // RegShift direction
	int bound; // Bound of RegShift's circulation

	int exec_time; // Execution time if it is stable
};