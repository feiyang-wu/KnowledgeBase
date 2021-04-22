# Image Processing Unit

## Introduction

## Background
 
## Architecture
### Overall Architecture
In general, IPU uses 2D shiftable registers array to deal with the data movement challenge. 
### IO Unit
IO Unit handles the memory management tasks between internal memory system and external memory. The instrucionts issued from Block Processing Engine's controller will execute for a few cycles here in order to handle the continuous data movement tasks. A state machine had been designed to accomplish the process by check the status stored in registers every cycle.   
Function:
- DMA
  - Target External Memory: DRAM on IPU Chips
  - Target Internal Memory: Tile Buffer / Data Buffer
- Data Reformatting
  - Image Format: Single Channel / NCHW / NHWC

Physical units:
- DMA Unit


### Block Processing Engine
#### Controller

Physical units:
- Instrcution Buffer

#### RISC-V Core
Function: 
- Do scalar instrucion in a reduced part of RISC-V 32I
- Expanded instruction to do special load/store operation for PEA.

Physical units:
- Attached Data Buffer

#### Processing Element Array

## Memory Management

### Instruction Set
#### Scalar Operation
- Load/Store operation starting signal between DRAM and Tile Buffer.
- Reserved for RISC-V I instruction set.
- Broadcast operation from Tile Buffer to Block Processing Engines' register files.
#### Memory Access
- Load/Store operation between Tile Buffer and Block Processing Engines' register files.
- Load/Store operation between Line Shared RAM and Block Processing Engines' register files.
<!-- - Shift operation between shiftable registers.
- Broadcast operation from Tile Buffer to Block Processing Engines' register files. -->
#### ALU Operation
- Normal Type: Instruction with 2/3/4 operands.
- I: Instruction with immediate operand.
- P: Every processing element will execute the operation depends on predicate bit.
#### Register Shift Operation
- Normal Type: Instruction with 2/3/4 operands.
- I: Instruction with immediate operand.
- P: Every processing element will execute the operation depends on predicate bit.

<!-- ## Question List
1. How to do things below in parallel? Instruction?
- Load/Store between DRAM and Tile Buffer
- Load/store between Tile Buffer and Register File
- Shift between Reigsters.  -->

## Data & Instruction in core
![](./img/inside_core_data.png)

![](./img/inside_core_inst.png)
## Algorithm
### Convolution
The main steps of the instruction sequence for Convolution is as below：
1. Load feature maps and weights from external memory by DMA.
    - Target Function Unit: IO Unit / Data Buffer
    - Progress: Set the state machine in BIU to start a data movement task between external memory and Tile Buffer Unit (TBU), set the write pointer for TBU every burst. 
    - Stall if: DMA is busy. 
2. Load feature map from Tile Buffer Unit to Processing Element Array
    - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: required data in TBU is not ready / target BIU's read port is busy / target register is ready
3. Load the first weight from Data Buffer and broadcast to Processing Element Array. 
   - Target Function Unit: Scalar Processor (SCP)
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus. 
   - Stall if: required data in Data Buffer is not ready
4. Load weight & Compute MAC & Register Shift
   - Target Function Unit: SCP & ALU & Shifter
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus & Compute MAC using two 16-bit ALU & Shift the feature map's data to the neighbers. 
   - Stall if: required feature map data is not ready
5. (Optional) Iterative Divide because of quantization
6. Store results from Register File to Tile Buffer Unit
   - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: target BIU's write port is busy / target TBU is full

<!-- - [x] `Google's Idea`可进行的扩展为如何完成大于5\*5而仍想保留16\*16规模的运算时，利用Line Shared Memory存储PE Array中存不下的数据。 -->

Instruction:
```
   dma_load [Tile Buffer Unit]
   dma_load [Data Memory]

LOOP:
   tbu_load ; ref image
   dm_broadcast_set r0 ; alt image, to pipeline

   load_buffer r0 & mul_acc r0, r1, r2, r3 & reg_shift
   load_buffer r0 & mul_acc r0, r1, r2, r3 & reg_shift

   ...

   load_buffer r0 & mul_acc r0, r1, r2, r3 & reg_shift ; do 3*3 to 5*5 times

   cmp r5, data_size/256
   jmp LOOP                                                      
```

### Block Matching
The main steps of the instruction sequence for Block Matching is as below：
1. Load reference image tile and alternative image tile from external memory by DMA.
    - Target Function Unit: IO Unit / Data Buffer
    - Progress: Set the state machine in BIU to start a data movement task between external memory and Tile Buffer Unit (TBU), set the write pointer for TBU every burst. 
    - Stall if: DMA is busy. 
2. Load reference image tile from Tile Buffer Unit to Processing Element Array
    - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: required data in TBU is not ready / target BIU's read port is busy / target register is ready
3. **Load the first pixel in alternative image tile from Data Buffer and broadcast to Processing Element Array.**
   - Target Function Unit: Scalar Processor (SCP)
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus. 
   - Stall if: required data in Data Buffer is not ready
4. Load pixel & Compute absolute subtraction and accumulate & Register Shift
   - Target Function Unit: SCP & ALU & Shifter
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus & Compute MAC using two 16-bit ALU & Shift the feature map's data to the neighbers. 
   - Stall if: required image data is not ready
5. Compare & Register Shift in row
6. Compare & Register Shift in column
7. Store results from Register File to Tile Buffer Unit
   - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: target BIU's write port is busy / target TBU is full

Instruction:
```
   dma_load [Tile Buffer Unit]
   dma_load [Data Memory]

LOOP:
   tbu_load ; ref image
   dm_broadcast_set r0 ; alt image, to pipeline

   sub_acc & reg_shift
   sub_acc & reg_shift

   ...

   sub_acc & reg_shift ; do 2*2 to 16*16 times

   find_min
   set_exec_mask
   find_min.p
   set_exec_mask
   store r1, r4

   jmp LOOP                                                      
```

### Matrix Multiply
The main steps of the instruction sequence for Block Matching is as below：
1. Load matrix A and matrix B from external memory by DMA.
    - Target Function Unit: IO Unit / Data Buffer
    - Progress: Set the state machine in BIU to start a data movement task between external memory and Tile Buffer Unit (TBU), set the write pointer for TBU every burst. 
    - Stall if: DMA is busy. 
2. Load matrix B from Tile Buffer Unit to Processing Element Array
    - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: required data in TBU is not ready / target BIU's read port is busy / target register is ready
3. Load the matrix A from Data Buffer and broadcast to Processing Element Array. 
   - Target Function Unit: Scalar Processor (SCP)
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus. 
   - Stall if: required data in Data Buffer is not ready
4. **Register Shift for initial**
5. Compute Multiply & Register Shift
   - Target Function Unit: SCP & ALU & Shifter
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus & Compute MAC using two 16-bit ALU & Shift the feature map's data to the neighbers. 
   - Stall if: required feature map data is not ready
6. (Optional) Iterative Divide because of quantization
7. Store results from Register File to Tile Buffer Unit
   - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: target BIU's write port is busy / target TBU is full

Instructions:
```
   dma_load [Tile Buffer Unit]
   dma_load [Data Memory]

LOOP:
   tbu_broadcast r0
   dm_col_broadcast_set r1
   load_buffer r1
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   load_buffer r1 & mul_acc r0, r1, r2, r3
   mul_acc r0, r1, r2, r3
   cmp r5, data_size/256
   jmp LOOP
```

### 2D DCT
The main steps of the instruction sequence for Block Matching is as below：
1. Load feature maps and weights from external memory by DMA.
    - Target Function Unit: IO Unit / Data Buffer
    - Progress: Set the state machine in BIU to start a data movement task between external memory and Tile Buffer Unit (TBU), set the write pointer for TBU every burst. 
    - Stall if: DMA is busy. 
2. Load feature map from Tile Buffer Unit to Processing Element Array
    - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: required data in TBU is not ready / target BIU's read port is busy / target register is ready
3. Load the first weight from Data Buffer and broadcast to Processing Element Array. 
   - Target Function Unit: Scalar Processor (SCP)
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus. 
   - Stall if: required data in Data Buffer is not ready
4. Load weight & Compute Multiply
   - Target Function Unit: SCP & ALU
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus & Compute MAC using two 16-bit ALU & Shift the feature map's data to the neighbers. 
   - Stall if: required feature map data is not ready
4. Load weight & Compute Multiply
   - Target Function Unit: SCP & ALU
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus & Compute MAC using two 16-bit ALU & Shift the feature map's data to the neighbers. 
   - Stall if: required feature map data is not ready
5. Store results from Register File to Tile Buffer Unit
   - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: target BIU's write port is busy / target TBU is full

Instructions:
```
   dma_load [Tile Buffer Unit] ; for images
   dma_load [Tile Buffer Unit] ; for parameters

   tbu_load r4 ; for parameters C
   tbu_load r5 ; for parameters CT

   tbu_load r0 ; for images

LOOP:
   load_buffer r0
   matrix_mul r0, r4
   matrix_mul r0, r5
   cmp r5, data_size/256
   jmp LOOP
```

### Histogram Equalization
<!-- The main steps of the instruction sequence for Histogram Equalization is as below：
1. Load feature maps and weights from external memory by DMA.
    - Target Function Unit: IO Unit / Data Buffer
    - Progress: Set the state machine in BIU to start a data movement task between external memory and Tile Buffer Unit (TBU), set the write pointer for TBU every burst. 
    - Stall if: DMA is busy. 
2. Load feature map from Tile Buffer Unit to Processing Element Array
    - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: required data in TBU is not ready / target BIU's read port is busy / target register is ready
3. Load the first weight from Data Buffer and broadcast to Processing Element Array. 
   - Target Function Unit: Scalar Processor (SCP)
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus. 
   - Stall if: required data in Data Buffer is not ready
4. Load weight & Compute MAC & Register Shift
   - Target Function Unit: SCP & ALU & Shifter
   - Progress: Load the weight in Data Buffer and broadcast it to every Compute Lane by instrcution bus & Compute MAC using two 16-bit ALU & Shift the feature map's data to the neighbers. 
   - Stall if: required feature map data is not ready
5. (Optional) Iterative Divide because of quantization
6. Store results from Register File to Tile Buffer Unit
   - Target Function Unit: Bus Interface Unit (BIU)
    - Progress: Set the state machine in BIU to start a data movement task, fill the status registers with values in instruction. Flip the ready bit for target register when the state machine turned to the end state. 
    - Stall if: target BIU's write port is busy / target TBU is full -->

Instructions:
```
; Statistic
   dma_load [Tile Buffer Unit]
   tbu_load

LOOP:
   load_buffer r0
   atomic_add r0
   addi r5, 1
   cmp r5, img_size/256
   jle LOOP

   lsr_load bin [r6 + 0-7]'s lower part
   lsr_load bin [r6 + 0-7]'s higher part
   add & reg_shift
   add & reg_shift
   add & reg_shift
   add & reg_shift
   dm_store bin [r6 + 0-7] ; 1 to 8 cycles

; Update
   dm_load r0, [r0]
   tbu_store r0
   jmp LOOP
```

Atomic Add do: 
1. Compute load address (lower 16 bit)
2. Load lower bit of CNT
3. Add & Compute load address (higher 16 bit)
4. store lower bit & if overflow then load higher bit of CNT
5. Add & Store higher bit

Or: Add a 32 bit adder near Line Shared RAM, and skip 2-5