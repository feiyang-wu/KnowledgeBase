import tensorflow as tf
import numpy as np

from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets("MNIST_data/",one_hot=True)

print("训练集数据的Shape：",mnist.train.images.shape)
print("测试集数据的Shape：",mnist.test.images.shape)
print("验证集数据的Shape：",mnist.validation.images.shape)

tf.reset_default_graph()
# 占位符
x=tf.placeholder(tf.float32,[None,784])
y=tf.placeholder(tf.float32,[None,10])
# 变量
W=tf.Variable(tf.random_normal([784,10]))
b=tf.Variable(tf.zeros([10]))
# 正向Softmax
pred=tf.nn.softmax(tf.matmul(x,W)+b)
# 反向
cost=tf.reduce_mean(-tf.reduce_sum(y*tf.log(pred),reduction_indices=1))
learning_rate=0.01
optimizer=tf.train.GradientDescentOptimizer(learning_rate).minimize(cost)
# 定义参数
training_epochs=25
batch_size=32
display_step=1

# 生成Saver用于后续保存模型
saver=tf.train.Saver()
model_path="model/model.ckpt"

# Train & Save Model

# with tf.Session() as sess:
#     sess.run(tf.global_variables_initializer())
#     # 循环迭代次数
#     for epoch in range(training_epochs):
#         avg_cost=0
#         total_batch=int(mnist.train.num_examples/batch_size)
#         # 循环batch数量
#         for i in range(total_batch):
#             batch_xs,batch_ys=mnist.train.next_batch(batch_size)
#             _,c=sess.run([optimizer,cost],feed_dict={x:batch_xs,y:batch_ys})
#             avg_cost+=c/total_batch
#         if(epoch+1) % display_step == 0:
#             print("Epoch:",'%04d' % (epoch+1), "cost=", "{:.9f}".format(avg_cost))

#     print("Finished!")
#     # 测试模型
#     correct_prediction=tf.equal(tf.argmax(pred,1),tf.argmax(y,1))
#     accuracy=tf.reduce_mean(tf.cast(correct_prediction,tf.float32))
#     print("Accuracy:",accuracy.eval({x:mnist.test.images,y:mnist.test.labels}))
#     # 保存模型
#     save_path=saver.save(sess,model_path)
#     print("Model saved in file: %s" % save_path)

# Load Model

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    # 读取模型
    saver.restore(sess,model_path)

    # 测试模型
    correct_prediction=tf.equal(tf.argmax(pred,1),tf.argmax(y,1))
    accuracy=tf.reduce_mean(tf.cast(correct_prediction,tf.float32))
    print("Accuracy:",accuracy.eval({x:mnist.test.images,y:mnist.test.labels}))
