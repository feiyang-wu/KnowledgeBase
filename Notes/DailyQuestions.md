# 常见问题
## 大端法与小端法
![小端法](https://upload.wikimedia.org/wikipedia/commons/thumb/e/ed/Little-Endian.svg/280px-Little-Endian.svg.png)

![大端法](https://upload.wikimedia.org/wikipedia/commons/thumb/5/54/Big-Endian.svg/280px-Big-Endian.svg.png)

## 并发(concurrency)与并行(parallellism)
- 顺序执行：你吃饭吃到一半，电话来了，你一直到吃完了以后才去接，这就说明你不支持**并发**也不支持**并行**。
- 并发：你吃饭吃到一半，电话来了，你停了下来接了电话，接完后继续吃饭，这说明你支持**并发**。
- 并行：你吃饭吃到一半，电话来了，你一边打电话一边吃饭，这说明你支持**并行**。
    [查看全文](https://blog.csdn.net/qq_27825451/article/details/78850336?utm_source=copy)

## FLOPS
FLOPS，即每秒浮点运算次数（亦称每秒峰值速度）是每秒所执行的浮点运算次数（英文：Floating-point operations per second，缩写：FLOPS）
英文单位|科学计数法
-|-
MFLOPS|10^6次浮点运算
GFLOPS|10^9次浮点运算
TFLOPS|10^12次浮点运算
PFLOPS|10^15次浮点运算
EFLOPS|10^18次浮点运算

## 国际单位制词头
### 倍数
英文缩写|英文全称|倍数
-|-|-
deka|da|10^1
hecto|h|10^2
kilo|k|10^3
mega|M|10^6
giga|G|10^9
tera|T|10^12
peta|P|10^15
exa|E|10^18
zetta|Z|10^21
yotta|Y|10^24

### 分数
英文缩写|英文全称|分数
-|-|-
deci|d|10^-1
centi|c|10^-2
milli|m|10^-3
micro|μ|10^-6
nano|n|10^-9
pico|p|10^-12
femto|f|10^-15
atto|a|10^-18
zepto|z|10^-21
yocto|y|10^-24

# 软件使用问题
## Git和Github
[简单使用Git和Github来管理自己的代码和读书笔记](https://my.oschina.net/bxxfighting/blog/378196)

## Markdown语法
[markdown基本语法](https://www.jianshu.com/p/191d1e21f7ed)
## Visual Studio 2017 找不到源文件stdio.h解决方法
原来的项目所采用windows SDK 已经发生了变化。因此解决的办法是：项目->属性->配置属性->常规->windows SDK版本。将其换成你现在的版本即可解决问题。