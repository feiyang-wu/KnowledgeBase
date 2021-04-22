# Instruction Set
Instrcutions can be divided into 4 types: Data movement instrcution, SIMD data process instructions, Mask data process instructions and flow control instructions. 
## Data Movement Instrcution
- dma_load: Start a load operation begin signal between DRAM and target memory. 
  - DRAM data base address: in special register
  - offset:
    - image number
    - image size
  - block size: 16-20
  - channel type: RGB
  - color depth: 8 bit
  - DRAM instruction base address: in special register
- dma_store: Start a store operation begin signal between DRAM and target memory. 
- tbu_load: Start a load operation begin signal between Tile Buffer Unit and target memory. 
- tbu_store: Start a store operation begin signal between Tile Buffer Unit and target memory. 
- tbu_broadcast: Start a broadcast operation begin signal between Tile Buffer Unit and target memory. 
## Data Process Instrcution
### Memory Access Part
- load_buffer: Load data from buffer.
- lsr_load: Load data between Line Shared RAM and register.
- lsr_load: Store data between Line Shared RAM and register.
- dm_load: Load data between Data Memory and register.
- dm_store: Store data between Data Memory and register.
- dm_col_broadcast: Start a broadcast operation begin signal between Data Memory in colmun first way.
### Data Computation Part
- add
- find_min
- atomic_add
- mul_acc: multiply and add
- sub_acc: subtraction and add

#### Bit Size
- Opcode: 6 bit
- Is Predicate: 1 bit
##### Register Number
- **Maximum:** 15 bits
- 5 bit per reg
- up to 3 register number
- for registers used more than 3, uses the first register as parameter. 
##### Memory Address
- for Line Shared RAM offset, up to 16 bit

### Register Shift Part
- Source Register: 2 bit (for shiftable reg00 - reg03)
- Destination Register: 2 bit (for shiftable reg00 - reg03)
- Stride: 4 bit (for 1 - 16)
- Loop bound: 1 bit (for 16 or 20)
- Direction: 2 bit (for up/down/left/right)
- (Optional) Selected Row/Column: 6 bit (1 bit for row/column, 5 bit for index 0 - 19)

<!-- ## Mask Data process instructions
- Is Predicate: 1 bit
### Line selected instructions
- atomic_add -->


## Flow Control Instrcution
- cmp
- jmp/jl/jg/jle/jge/je/jne
### Mask selected instructions
- set_exec_mask