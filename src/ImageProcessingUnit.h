#pragma once
#include"config.h"
#include"DRAM.h"
#include"Instruction.h"
#include"ImageConveyor.h"
#include"SliceBuffer.h"
#include"SquareConveyor.h"
#include"SquareProcessor.h"

class ImageProcessingUnit {
public:
	ImageProcessingUnit(DRAM& dram_instance) :imc(dram_instance, sbp)
	{
		for (int i = 0; i < SliceBufferNum; i++)
		{
			sbp.push_back(SliceBuffer(i));
		}

		for (int i = 0; i < SquareProcessorNum; i++)
		{
			sqc_list.push_back(SquareConveyor(sbp, i));
			sqp_list.push_back(SquareProcessor(sqc_list[i], i));
		}
	};

	void cycle();

	ImageConveyor imc;
	vector<SliceBuffer> sbp;
	vector<SquareConveyor> sqc_list;
	vector<SquareProcessor> sqp_list;
};