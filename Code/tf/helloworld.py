import os
import tensorflow as tf

# 静默输出
# os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# Session
# Session里面放了很多资源，用完要关掉，有两种方法：
## 方法1：用`close()`方法
def session_test1():
    hello = tf.constant('Hello, Tensorflow!')
    sess = tf.compat.v1.Session() # tf.Session() API 更新为 tf.compat.v1.Session()了
    print(sess.run(hello))
    sess.close()

## 方法2：用with来控制他关闭
def session_test2():
    a = tf.constant(3)
    b = tf.constant(4)
    with tf.compat.v1.Session() as sess:
        print("Add: %i" % sess.run(a+b))
        print("Mul: %i" % sess.run(a*b))

# placeholder
def placeholder_test():
    a = tf.compat.v1.placeholder(tf.int16)
    b = tf.compat.v1.placeholder(tf.int16)
    add = tf.add(a,b)
    mul = tf.multiply(a,b)
    with tf.compat.v1.Session() as sess:
        print("Add: %i" % sess.run(add,feed_dict={a:3,b:4}))
        print("Mul: %i" % sess.run(mul,feed_dict={a:3,b:4}))
        print(sess.run([add,mul],feed_dict={a:3,b:4}))

# 指定GPU运算
def using_GPU_test():
    a = tf.compat.v1.placeholder(tf.int16)
    b = tf.compat.v1.placeholder(tf.int16)
    add = tf.add(a,b)
    with tf.compat.v1.Session() as sess:
        with tf.device("/GPU:0"):
            print("Add: %i" % sess.run(add,feed_dict={a:3,b:4}))

# 限制GPU使用资源
def limit_GPU_test():
    # config = tf.compat.v1.ConfigProto(log_device_placement=True,allow_soft_placement=True)
    # sess = tf.compat.v1.Session(config=config)
    gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.1) # 允许使用显存大小的百分比
    config = tf.ConfigProto(gpu_options=gpu_options)
    sess = tf.compat.v1.Session(config=config)
    a = tf.compat.v1.placeholder(tf.int16)
    b = tf.compat.v1.placeholder(tf.int16)
    add = tf.add(a,b)
    print("Add: %i" % sess.run(add,feed_dict={a:3,b:4}))
    sess.close()

# 保存模型
def save_model_test():
	saver = tf.train.Saver()
	with tf.Session() as sess:
		sess.run(tf.global_variables_initializer())
		# 将数据放入模型进行训练
		saver.save(sess, "save_model")

# Running test below:
save_model_test()
 

