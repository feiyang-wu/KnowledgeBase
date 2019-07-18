# FloatPIM

## PIM存在的问题
PIM可能用于处理数据搬移问题。
1. PIM只能处理定点，而CNN需要浮点来支持更高精度的运算
2. ADC和DAC花去了大量的面积和功耗

## FloatPIM特性
1. 直接支持浮点CNN，使用模拟信号存储但不做数模转换；
2. FloatPIM直接在数字信号上做PIM操作，所有计算通过1位的双极电阻装置（bipolar resistive devices）上实现的或非门来进行。这样避免了数模转换和多位忆阻器；
3. FloatPIM工作在两个模式下：
    1. 计算阶段：并行运算矩阵乘/卷积
    2. 数据搬移阶段：执行流水线，行并行的从邻近内存块中进行数据搬移
4. 实验范围：在FloatPIM上跑了AlexNet, VGGNet, GoogleNet, and SqueezeNet for ImageNet dataset，并和PipeLayer PIM accelerator，ISAAC PIM accelerator进行了对比。

## Digital PIM
使用或非门做运算，即有1出0，全0出1