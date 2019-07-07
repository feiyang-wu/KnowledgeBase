# General Usage of Linux ( Ubuntu )
## 1 查看系统信息
### 1.1 查看内核版本
- 方法1

```$ cat /proc/version```

- 方法2

```$ uname```

|参数|全写|释意|翻译|
|:---:|:-:|-|-|
|-a|--all|print all information, in the following order, except omit -p and -i if unknown|按序打印下述所有属性|
|-s|--kernel-name|print the kernel name|内核名称|
|-n|--nodename|print the network node hostname|网络节点hostname|
|-r|--kernel-release|print the kernel release|内核发行版本|
|-v|--kernel-version|print the kernel version|内核版本|
|-m|--machine|print the machine hardware name|机器硬件名|
|-p|--processor|print the processor type (non-portable)|处理器类型|
|-i|--hardware-platform|print the hardware platform (non-portable)|硬件平台|
|-o|--operating-system|print the operating system|操作系统|
|--help|--help|display this help and exit|帮助|
|--version|--version|output version information and exit|版本信息|

### 1.2 查看Linux版本
```$ cat /etc/issue```

### 1.3 查看系统位数
```
$ getconf WORD_BIT
$ file /bin/bash
```

### 1.4 查看GCC版本
```$ gcc --version```

## 2 常用命令
|命令|注释|
|-|-|
|`$ wget`|下载命令|
|`$ mkdir`|创建文件夹|
|`$ cp -rf <source>/* <dest>`|将整个文件夹及子文件复制至另一个文件夹|
|`$ mv <source>/<file> <dest>`|将文件剪切到另一个文件夹下|
|`$ mv <source>/* <dest>`|将整个文件夹剪切到另一个文件夹下|
|`$ rm -f <dest>/<file>`|删除指定文件|
|`$ rm -rf <dest>`|清空文件夹|
|`$ pwd`|显示当前的工作目录|
|`$ find <dest> -name "<string>"`|按文件名查找文件|

**使用时替换`<source>`，`<file>`，`<dest>`,<string>**

## 3 常用操作
### 3.1 修改全局/用户环境变量
详见[Ubuntu系统环境变量详解](https://www.linuxidc.com/Linux/2016-09/135476.htm)
```
$ vim /etc/profile      # 修改全局环境变量
$ vim ./~bashrc         # 修改用户环境变量
```
加入
```
export <path_name>=<path>
```
**使用时替换`<path_name>`，`<path>`**

### 3.2 修改GCC，G++版本
1. 打开`.bashrc`
```
$ vim ~/.bashrc
```
2. 写入PATH
```
export PATH=/home/<username>/bin:$PATH
```
**使用时替换`<username>`**

3. 建立GCC，G++链接
```
$ mkdir ~/bin
$ ln -s /usr/bin/gcc-<version> ~/bin/gcc
$ ln -s /usr/bin/g++-<version> ~/bin/g++
```

4. 查看链接
```
$ cd ~/bin
$ ls -li
```