#include "ImageProcessingUnit.h"

void ImageProcessingUnit::cycle()
{
	imc.cycle();
	for (int i = 0; i < SquareProcessorNum; i++)
	{
		sqp_list[i].cycle();
	}
}
