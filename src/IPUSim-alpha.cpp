#include"DRAM.h"
#include"ImageProcessingUnit.h"


//class IPUSim {
//public:
//	IPUSim() :dram(2*20*20),io_unit() 
//	{
//		io_unit.dram = &dram;
//		StencilProcessor tmpstp(0);
//		stp_array.push_back(tmpstp);
//		SheetGenerator tmpshg(0);
//		shg_array.push_back(tmpshg);
//		LineBufferUnit tmplbu0(2000, 8, 0, 0, 20, 20, 20);
//		LineBufferUnit tmplbu1(25, 1, 1, 1, 5, 0, 0);
//		vector<dataType> tmpweight = { 2,4,5,4,2,4,9,12,9,4,5,12,15,12,5,4,9,12,9,4,2,4,5,4,2 };
//		tmplbu1.data[0] = tmpweight;
//		LineBufferUnit tmplbu2(2000, 8, 2, 2, 16, 16, 16);
//		lbu_array.push_back(tmplbu0);
//		lbu_array.push_back(tmplbu1);
//		lbu_array.push_back(tmplbu2);
//
//		for (int i = 0; i < 20; i++)
//		{
//			for (int j = 0; j < 20; j++)
//			{
//				dram.data[i * 20 + j] = i * 20 + j;
//			}
//		}
//
//		Instruction inst0;
//		inst0.inst_type = InstructionType::LDDRLB;
//		inst0.src_addr = 0;
//		inst0.img_x = 0;
//		inst0.img_y = 0;
//		inst0.lbu = &lbu_array[0];
//		inst0.bank_num = 0;
//		inst0.size = 20;
//		inst0.delay_time = 50;
//
//		io_unit.inst_buf.push(inst0);
//
//		
//
//	};
//
//	void cycle();
//
//	vector<StencilProcessor> stp_array;
//	vector<SheetGenerator> shg_array;
//	vector<LineBufferUnit> lbu_array;
//	IOUnit io_unit;
//	DRAM dram;
//};
//
//void IPUSim::cycle()
//{
//	for (auto i : stp_array)
//	{
//		i.cycle();
//	}
//	for (auto i : shg_array)
//	{
//		i.cycle();
//	}
//	io_unit.cycle();
//}


int main()
{
	/*IPUSim instance;
	for(int i=0;i<=51;i++)
	{
		cout << "Cycle #" << i << endl;
		instance.cycle();
	}
	Instruction inst1;
	inst1.inst_type = InstructionType::LDLBSG;
	inst1.shg = &instance.shg_array[0];
	inst1.bank_num = 0;
	inst1.size = 20;
	inst1.src_addr = 0;
	inst1.dst_addr = 0;
	inst1.delay_time = 20;

	instance.stp_array[0].inst_buf.push(inst1);
	for (int i = 52; i < 200; i++)
	{
		cout << "Cycle #" << i << endl;
		instance.cycle();
	}*/
	
	DRAM dram;
	for (int i = 0; i < 400; i++)
	{
		dram.data[i] = 159;
	}
	vector<singleByte> weight = { 2,4,5,4,2,4,9,12,9,4,5,12,15,12,5,4,9,12,9,4,2,4,5,4,2 };
	for (int i = 0; i < 25; i++)
	{
		dram.data[400+i] = weight[i];
	}

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			cout << static_cast<int>(dram.data[20*i+j])<< " " ;
		}
		cout << endl;
	}

	ImageProcessingUnit ipu(dram);
	Instruction inst;
	inst.inst_type = InstructionType::LDDRSB;
	inst.height_size = 20;
	inst.width_size = 20;
	inst.height_img = 20;
	inst.width_img = 20;
	inst.sbu_idx = 0;
	inst.exec_time = 50;
	ipu.imc.inst_buf.push(inst);

	// Instruction inst;
	inst = Instruction();
	inst.inst_type = InstructionType::LDSBSC;
	inst.square_size = 20;
	inst.sbu_idx = 0;
	inst.exec_time = 50;
	ipu.sqp_list[0].inst_buf.push(inst);

	// Instruction inst;
	inst = Instruction();
	inst.inst_type = InstructionType::LDSCRF;
	inst.dst_reg_x_shiftable = true;
	inst.square_size = 20;
	inst.sbu_idx = 0;
	inst.exec_time = 20;
	ipu.sqp_list[0].inst_buf.push(inst);

	// Instruction inst;
	inst = Instruction();
	inst.inst_type = InstructionType::STRFSC;
	inst.src_reg_a_shiftable = true;
	inst.square_size = 16;
	inst.exec_time = 16;
	ipu.sqp_list[0].inst_buf.push(inst);
	
	// Instruction inst;
	inst = Instruction();
	inst.inst_type = InstructionType::STSCSB;
	inst.dst_reg_x_shiftable = true;
	inst.square_size = 16;
	inst.sbu_idx = 2;
	inst.exec_time = 50;
	ipu.sqp_list[0].inst_buf.push(inst);

	// Instruction inst;
	inst = Instruction();
	inst.inst_type = InstructionType::STSBDR;
	inst.height_size = 16;
	inst.width_size = 16;
	inst.height_img = 16;
	inst.width_img = 16;
	inst.dst_addr = 425;
	inst.square_size = 16;
	inst.sbu_idx = 2;
	inst.exec_time = 50;
	ipu.imc.inst_buf.push(inst);

	for (int i = 0; i < 150; i++)
	{
		cout << "Cycle #" << i << ": " << endl;
		ipu.cycle();
		cout << endl;
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			cout << static_cast<int>(dram.data[425 + 16 * i + j]) << " ";
		}
		cout << endl;
	}

    std::cout << "Hello IPUSim!\n";
}


