# 环境搭建相关
## PATH
### 全局/用户环境变量
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

### 控制GCC，G++版本
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
## Miniconda
### Conda管理
- 查看版本：`conda --version`
- 升级版本：`conda update conda`
### 环境管理
- 查看所有环境：`conda info -e`
- 创建环境：`conda create -n <environment name> <packages>`
- 激活环境：`conda activate <environment name>`
- 注销环境：`conda deactivate`
### 包管理
- 查看所有包：`conda list`
- 安装新的包：`conda install [-n <environment name>] <packages>`
- 移除某个包：`conda remove [-n <environment name>] <packages>`

**使用时替换`<environment name>`，`<packages>`**