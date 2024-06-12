# Video
## 视频压制与解压
### 关键词
缩写|全名|中文释义
-|-|-
ITU|International Telecommunications Union|国际电信联盟
VECG|Video Coding Experts Group|视频编码专家组
ISO|International Standards Organization|国际标准化组织
MPEG|Motion Picture Experts Group|运动图像专家组
### 数据压缩分类
- 无损压缩
    - 压缩前解压缩后图像完全一致X=X'
    - 压缩比低(2:1~3:1)
    - 例如：Winzip，JPEG-LS
- 有损压缩
    - 压缩前解压缩后图像不一致X≠X'
    - 压缩比高(10:1~20:1)
    - 例如：MPEG-2，H.264/AVC，AVS
### 编解码器
- 编码器（Encoder）：压缩信号的设备或程序
- 解码器（Decoder）：解压缩信号的设备或程序
- 编解码器(Codec)：编解码器对
### 编解码流程
-> 预测 -> 变换 -> 量化 -> 熵编码 -> 信道传输 -> 熵解码 -> 反量化 -> 反变换 -> 预测 ->
- 预测：去除空间冗余和时间冗余，如帧内预测，帧间预测
- 变换：去除空间冗余，如DCT，小波变换
- 量化：去除视觉冗余，通过降低图像质量提高压缩比
- 熵编码：去除编码冗余，如变长编码，算术编码
### 