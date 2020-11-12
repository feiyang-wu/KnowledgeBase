#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<string>
using namespace std;

typedef int addrType;
typedef uint8_t singleByte;
typedef int16_t doubleByte;

/********************************
* IPU Set Up Resources
********************************/
constexpr int SliceBufferNum = 3;
constexpr int SquareProcessorNum = 1;

/********************************
* Instruction
********************************/
enum class Direction
{
	left, // 0
	right, // 1
	up,
	down,
	NONE
};

enum class InstructionType
{
	NONE,
	LDDRSB, // Load data from DRAM to Slice Buffer
	STSBDR, // Store data from Line Buffer to DRAM
	LDSBSC, // Load data from Slice Buffer to Square Conveyor
	LDSBW, // Load data from Line Buffer to Sheet Generator's weight buffer
	STSCSB, // Store data from Square Conveyor to Slice Buffer
	LDSCRF, // Load data from Sheet Generator to Register File
	LDWRF, // Broadcast data from Sheet Generator to Register File
	STRFSC, // Store data from Register File to Sheet Generator
	MAD, // Multiply and add
	IDV, // Iterative divide, 8 cycles for 16bit
	SHIFT
};

/********************************
* Image Conveyor Delay
********************************/
constexpr int ImageConveyorFetchDelay = 1;
constexpr int ImageConveyorDecodeDelay = 1;
constexpr int ImageConveyorIssueDelay = 1;

/********************************
* Square Processor Delay
********************************/
constexpr int SquareProcessorFetchDelay = 1;
constexpr int SquareProcessorDecodeDelay = 1;
constexpr int SquareProcessorIssueDelayToPE = 1;
constexpr int SquareProcessorIssueDelayToRF = 1;
constexpr int SquareProcessorIssueDelayToSC = 1;


/********************************
* Memory Size
********************************/
constexpr int DRAMSize = 2000000;

constexpr int PESizePerSTP = 16;
constexpr int RFSizePerSTP = 20;

constexpr int RegSizePerRF = 4;
constexpr int RegSizePerPE = 16;

constexpr int SliceBufferBankNum = 8;
constexpr int SliceBufferBankSize = 2000;

constexpr int InBufferBankSize = 40;
constexpr int InBufferBankNum = 20;

constexpr int OutBufferBankSize = 32;
constexpr int OutBufferBankNum = 16;

constexpr int WeightBufferBankSize = 32;
constexpr int WeightBufferBankNum = 1;

constexpr addrType DRAMBaseAddress = 0x0;

