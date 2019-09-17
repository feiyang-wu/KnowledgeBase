# Reconfigurable Acceleration of 3D-CNNs for Human Action Recognition with Block Floating-Point Representation

## Contribution
1. 可自定义的用于动作识别(human action recognition, HAR)的3D-CNN的FPGA实现，快准+不需要重训练；
1. 针对locality的优化 & 3D块对齐等优化策略；
1. 实验性(?)地分析单精度和双精度下BFP的表现，同样准确率下单精度比双精度快一倍；
1. 提供了自动设置网络自定义的参数来平衡性能-准确率的工具。

## BFP
BFP即块浮点数，就是把一个block的浮点数的阶码中取最大的阶码存起来作为整个block的阶码，其他数的尾数根据这个**块阶码**scale一下再存。
> 但是一个块如果太大了，就会导致内部浮点数差值较大，scale之后会出现精度损失，因此选择较小的块会获得较小的精度损失。

