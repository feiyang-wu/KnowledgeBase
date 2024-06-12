# Tensorflow基础知识
## 神经网络
### 激活函数
- Sigmoid
- Tanh
- ReLU
- SoftPlus
### softmax
解决分类问题
### 损失函数
#### MSE
Mean Squared Error，均值平方差，也称均方误差，值越小越好。类似的有RMSE，MAD。
TensorFlow没有单独的MSE，要自己写。
#### 交叉熵
也是值越小越好，TF写好了，有：
- Sigmoid交叉熵
- softmax交叉熵：需要one-hot编码
- Sparse交叉熵：和前者一样，但是不需要one-hot编码，但要求类型从0开始连续编码
- 加权Sigmoid交叉熵
#### 梯度下降
- 退化学习率

## 多层神经网络
- 隐藏层
- 正则化
    - L1范数：所有学习参数w的绝对值的和，得手写`tf.reduce_sum(tf.abs(w))`
    - L2范数：所有学习参数w的平方和然后求平方根，可以直接调用`tf.nn.l2_loss(t, name=None)`
- dropout
## 卷积神经网络
### 卷积
- 步长
- 窄卷积：卷积后比原图小
- 同卷积：和原图一样大，需要padding一圈且步长固定为1
- 全卷积：也叫反卷积，把原图变大
### 池化
- 均值池化
- 最大池化