import tensorflow as tf
import numpy as np

train_X = np.linspace(-1,1,1000)
train_Y = 2*train_X + np.random.randn(*train_X.shape)*0.3

# 创建模型
# 占位符
X = tf.placeholder("float")
Y = tf.placeholder("float")
# 模型参数
W = tf.Variable(tf.random_normal([1]), name="weight")
b = tf.Variable(tf.zeros([1]), name="bias")
# 前向结构
z = tf.multiply(X, W) + b
# 反向优化
cost = tf.reduce_mean(tf.square(Y - z))
learning_rate = 0.01
optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(cost)

# 初始化所有变量
init = tf.global_variables_initializer()

# 定义参数
training_epochs = 100
display_step = 10

# 启动Session
with tf.Session() as sess:
    sess.run(init)
    # 给模型喂数据
    for epoch in range(training_epochs):
        for (x,y) in zip(train_X,train_Y):
            sess.run(optimizer,feed_dict={X:x, Y:y})

        if epoch%display_step == 0:
            loss = sess.run(cost,feed_dict={X:train_X, Y:train_Y})
            print("Epoch:",epoch+1,"Cost=",loss,"W=",sess.run(W),"b=",sess.run(b))
    # 展示结果
    print("Finished!")
    print("Cost=",sess.run(cost,feed_dict={X:train_X, Y:train_Y}),"W=",sess.run(W),"b=",sess.run(b))