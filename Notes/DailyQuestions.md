# 常见易混淆问题
## 大端法与小端法
![小端法](https://upload.wikimedia.org/wikipedia/commons/thumb/e/ed/Little-Endian.svg/280px-Little-Endian.svg.png)

![大端法](https://upload.wikimedia.org/wikipedia/commons/thumb/5/54/Big-Endian.svg/280px-Big-Endian.svg.png)

## 并发(concurrency)与并行(parallellism)
- 顺序执行：你吃饭吃到一半，电话来了，你一直到吃完了以后才去接，这就说明你不支持**并发**也不支持**并行**。
- 并发：你吃饭吃到一半，电话来了，你停了下来接了电话，接完后继续吃饭，这说明你支持**并发**。
- 并行：你吃饭吃到一半，电话来了，你一边打电话一边吃饭，这说明你支持**并行**。
    [查看全文](https://blog.csdn.net/qq_27825451/article/details/78850336?utm_source=copy)

# 软件使用问题
## Git和Github
[简单使用Git和Github来管理自己的代码和读书笔记](https://my.oschina.net/bxxfighting/blog/378196)

## Markdown语法
[markdown基本语法](https://www.jianshu.com/p/191d1e21f7ed)
## Visual Studio 2017 找不到源文件stdio.h解决方法
原来的项目所采用windows SDK 已经发生了变化。因此解决的办法是：项目->属性->配置属性->常规->windows SDK版本。将其换成你现在的版本即可解决问题。