# -*- coding: utf-8 -*-
"""
Training a Classifier
=====================

This is it. You have seen how to define neural networks, compute loss and make
updates to the weights of the network.

Now you might be thinking,

What about data?
----------------

Generally, when you have to deal with image, text, audio or video data,
you can use standard python packages that load data into a numpy array.
Then you can convert this array into a ``torch.*Tensor``.

-  For images, packages such as Pillow, OpenCV are useful
-  For audio, packages such as scipy and librosa
-  For text, either raw Python or Cython based loading, or NLTK and
   SpaCy are useful

Specifically for vision, we have created a package called
``torchvision``, that has data loaders for common datasets such as
Imagenet, CIFAR10, MNIST, etc. and data transformers for images, viz.,
``torchvision.datasets`` and ``torch.utils.data.DataLoader``.

This provides a huge convenience and avoids writing boilerplate code.

For this tutorial, we will use the CIFAR10 dataset.
It has the classes: ‘airplane’, ‘automobile’, ‘bird’, ‘cat’, ‘deer’,
‘dog’, ‘frog’, ‘horse’, ‘ship’, ‘truck’. The images in CIFAR-10 are of
size 3x32x32, i.e. 3-channel color images of 32x32 pixels in size.

.. figure:: /_static/img/cifar10.png
   :alt: cifar10

   cifar10


Training an image classifier
----------------------------

We will do the following steps in order:

1. Load and normalizing the CIFAR10 training and test datasets using
   ``torchvision``
2. Define a Convolution Neural Network
3. Define a loss function
4. Train the network on the training data
5. Test the network on the test data

1. Loading and normalizing CIFAR10
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Using ``torchvision``, it’s extremely easy to load CIFAR10.
"""
import torch
import torchvision
import torchvision.transforms as transforms

# 使用GPU训练和验证
# device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu") # gpu
# print(device) # gpu
########################################################################
# The output of torchvision datasets are PILImage images of range [0, 1].
# We transform them to Tensors of normalized range [-1, 1]

# 指向cifar-10-batches-py目录的上一级目录
data_dir='~/data_base'
transform = transforms.Compose(
    [transforms.ToTensor(),
     transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

trainset = torchvision.datasets.CIFAR10(root=data_dir, train=True,
                                        download=True, transform=transform)
trainloader = torch.utils.data.DataLoader(trainset, batch_size=4,
                                          shuffle=True, num_workers=2)

testset = torchvision.datasets.CIFAR10(root=data_dir, train=False,
                                       download=True, transform=transform)
testloader = torch.utils.data.DataLoader(testset, batch_size=4,
                                         shuffle=False, num_workers=2)

classes = ('plane', 'car', 'bird', 'cat',
           'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

########################################################################
# Let us show some of the training images, for fun.

import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
import numpy as np



# functions to show an image


def imshow(img):
    img = img / 2 + 0.5     # unnormalize
    npimg = img.numpy() # 取值在[0, 1]之间的浮点数
    plt.imshow(np.transpose(npimg, (1, 2, 0))) # 此行在远程命令行操作模式下会引发异常

# get some random training images
dataiter = iter(trainloader)
images, labels = dataiter.next()

# show images
# 注释, 远程执行时会导致报错
#imshow(torchvision.utils.make_grid(images))
# print labels
#print(' '.join('%5s' % classes[labels[j]] for j in range(4)))


########################################################################
# 2. Define a Convolution Neural Network
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# Copy the neural network from the Neural Networks section before and modify it to
# take 3-channel images (instead of 1-channel images as it was defined).

#from torch.autograd import Variable # v0.4版中正式去掉了该句, 原因是Variable类和Tensor类不再做区分
import torch.nn as nn
import torch.nn.functional as F


# pytorch中用到torch.autograd.Variable类型对象一般有两种情况:
# (1)Module类中的模型参数, 输入Variable类的派生类Parameter.
#    参数是用于被训练(迭代更新)的, 一般在 Module.__init__() 方法中定义;
# (2)Module的forward()方法的input和output(即计算图的输入输出), 他们都是Variabel类对象.
#    input是数值量的源头, output是计算图的载体, Module.backward()方法通过output来计算计算图中各个叶子节点的梯度.


# 把特征提取部分定义为一个Module派生类中
class Net(nn.Module):

    # 每层的Module对象创建的时候就会完成以下工作: (1)将待求梯度参数加入Module._parameters; (2)将每层的数值参数随机初始化.
    # 由于Net类中组成网络的"基本层"在各自类定义中都已经定义了参数, 所以Net.__init__()无需再向self._parameters字典中加入Parameter对象
    def __init__(self):
        super(Net, self).__init__()

        # nn.Conv2d和nn.MaxPool2d都是nn.Module的派生类

        # kernel_size=(5, 5), stride=1, padding=None, (w_ou, h_ou)=(w_in, h_in) - 4 
        self.conv1 = nn.Conv2d(3, 6, 5) # nn.Conv2d.__init__(self, in_channels, out_channels, kernel_size, stride=1, padding=0, dilation=1, groups=1, bias=True)

        # kernel_size=(2, 2), stride=2, (w_ou, h_ou)=(w_in, h_in)/2
        # 注意到该层在forward()方法中被两次使用, 估计有参数的层可能不能这样用
        self.pool = nn.MaxPool2d(2, 2) # nn.MaxPool2d.__init__(self, kernel_size, stride=None, padding=0, dilation=1, return_indices=False, ceil_mode=False)

        # kernel_size=(5, 5), stride=1, padding=None, (w_ou, h_ou)=(w_in, h_in) - 4
        self.conv2 = nn.Conv2d(6, 16, 5)

        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        # 这个其实相当于Softmax层的全连接部分, 过去我们理解的所谓"Softmax层"实际是"全连接层(输出维数和分类问题类别数相等) + CrossEntropy损失计算函数"
        # CrossEntropy损失计算函数现在被单独作为_Loss类的子类
        # 这样的好处是, 损失类没有任何需要求导的参数, 在实现时不必再考虑向Module._paramter字典中添加参数
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        # Conv2d: kernel_size=(5, 5), stride=1, padding=None, (w_ou, h_ou)=(w_in, h_in) - 4 = (32, 32) - 4 = (28, 28)
        # MaxPool2d: kernel_size=(2, 2), stride=2, (w_ou, h_ou)=(w_in, h_in)/2 = (28, 28)/2 = (14, 14)
        x = self.pool(F.relu(self.conv1(x)))

        # Conv2d: kernel_size=(5, 5), stride=1, padding=None, (w_ou, h_ou)=(w_in, h_in) - 4 = (14, 14) - 4 = (10, 10)
        # MaxPool2d: kernel_size=(2, 2), stride=2, (w_ou, h_ou)=(w_in, h_in)/2 = (10, 10)/2 = (5, 5)
        x = self.pool(F.relu(self.conv2(x)))

        # torch.Tensor的reshape, 注意nn.Linear类的forward()方法的实现采用funtional.linear()函数, 该函数对于二维和其他维输入采用不同的矩阵运算符实现
        x = x.view(-1, 16 * 5 * 5) # 留出一维是否是考虑到x可能是mini-batch??
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

# 创建特征提取部分的Module类对象
net = Net()
#net.to(device) # GPU

########################################################################
# 3. Define a Loss function and optimizer
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# Let's use a Classification Cross-Entropy loss and SGD with momentum

import torch.optim as optim

criterion = nn.CrossEntropyLoss() # nn.CrossEntropyLoss也是nn.Module的派生类

# net.parameters()返回一个generator, 能够遍历产生net的所有子Module的所有参数(每个参数是一个Variable对象)
# 由于Net类中组成网络的"基本层"都已经定义了参数
# 所以虽然Net.__init__()中并未直接或间接的想self._parameters字典中加入参数, 但net.parameters()仍然能够通过递归遍历所有子Module来找到全部参数
optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

########################################################################
# 4. Train the network
# ^^^^^^^^^^^^^^^^^^^^
#
# This is when things start to get interesting.
# We simply have to loop over our data iterator, and feed the inputs to the
# network and optimize

#for epoch in range(2):  # loop over the dataset multiple times
for epoch in range(100):  # loop over the dataset multiple times

    running_loss = 0.0
    for i, data in enumerate(trainloader, 0):

        # 0.准备工作
        # get the inputs
        inputs, labels = data
        #inputs, labels = inputs.to(device), labels.to(device) # GPU

        # wrap them in Variable
        #inputs, labels = Variable(inputs), Variable(labels) # v0.4版本开始此句消失

        # zero the parameter gradients
        optimizer.zero_grad() # 如果不置为0, Variable的梯度在每次 backward() 的时候都会累加

        # 1.获得计算图及其输出Variable

        # 1.1.获得网络结构的输出Variable
        # forward + backward + optimize
        # 调用Module类的子类Net的__call__()方法, 执行Net.forward()方法, 得到通过输入inputs计算输出的运算表达式图(Graph)outputs
        outputs = net(inputs)

        # 1.2.获得完整计算图的输出Variable
        # 调用Module类的子类criterion的__call__()方法, 其中会执行用户定义的criterion.forward()方法, forward()方法的输入参数与__call__()方法的输入参数完全相同
        # 得到通过输入inputs和label, 计算输出的运算表达式图(Graph)loss
        loss = criterion(outputs, labels)

        # 2.优化和更新参数

        # 2.1.计算参数梯度
        # 执行计算图输出的Variable对象的backward()方法, 获得关于计算图中各个参数的梯度数值
        loss.backward()

        # 2.2.进行一次参数数值的更新
        # Optimizer对象的setp()方法由具体子类实现
        # step()方法并不负责计算导数(计算导数的任务是由计算图(Graph)的输出Variable调用自身的backward()方法完成的)
        # step()负责检查保存在Optimizer对象中的所有参数, 根据已经计算好的Variable.grad.data, 计算出参数的变化值, 然后根据该值修改参数的值
        # 计算过程中会利用Optimizer对象所保存的每个参数的'weight_decay', 'lr'等详细优化方法信息
        optimizer.step()

        # print statistics
        # torch.Tensor.item(), 注意只适用于scalar型的Tensor，适合返回loss, acc等
        running_loss += loss.item() # v0.4之前是: running_loss += loss.data[0]
        if i % 2000 == 1999:    # print every 2000 mini-batches
            print('[%d, %5d] loss: %.3f' %
                  (epoch + 1, i + 1, running_loss / 2000))
            running_loss = 0.0

print('Finished Training')

########################################################################
# 5. Test the network on the test data
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#
# We have trained the network for 2 passes over the training dataset.
# But we need to check if the network has learnt anything at all.
#
# We will check this by predicting the class label that the neural network
# outputs, and checking it against the ground-truth. If the prediction is
# correct, we add the sample to the list of correct predictions.
#
# Okay, first step. Let us display an image from the test set to get familiar.

dataiter = iter(testloader)
images, labels = dataiter.next()
# images, labels = images.to(device), labels.to(device) # GPU

# print images
# 注释下面两句, 远程执行时会导致报错
#imshow(torchvision.utils.make_grid(images))
#print('GroundTruth: ', ' '.join('%5s' % classes[labels[j]] for j in range(4)))

########################################################################
# Okay, now let us see what the neural network thinks these examples above are:

outputs = net(images) # v0.4之前是: outputs = net(Variable(images))

########################################################################
# The outputs are energies for the 10 classes.
# Higher the energy for a class, the more the network
# thinks that the image is of the particular class.
# So, let's get the index of the highest energy:
_, predicted = torch.max(outputs, 1) # v0.4之前是: _, predicted = torch.max(outputs.data, 1)

#print('Predicted: ', ' '.join('%5s' % classes[predicted[j]]
#                              for j in range(4)))

########################################################################
# The results seem pretty good.
#
# Let us look at how the network performs on the whole dataset.

correct = 0
total = 0
with torch.no_grad(): # v0.4新增
    for data in testloader:
        images, labels = data
        #images, labels = images.to(device), labels.to(device) # GPU
        outputs = net(images) # v0.4之前是: outputs = net(Variable(images))
        _, predicted = torch.max(outputs.data, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item() # v0.4之前是: correct += (predicted == labels).sum()

print('Accuracy of the network on the 10000 test images: %d %%' % (
    100 * correct / total))

########################################################################
# That looks waaay better than chance, which is 10% accuracy (randomly picking
# a class out of 10 classes).
# Seems like the network learnt something.
#
# Hmmm, what are the classes that performed well, and the classes that did
# not perform well:

class_correct = list(0. for i in range(10))
class_total = list(0. for i in range(10))
with torch.no_grad(): # 此句在v0.4以前的例子中没有出现
    for data in testloader:
        images, labels = data
        #images, labels = images.to(device), labels.to(device) # GPU
        outputs = net(images)
        _, predicted = torch.max(outputs, 1)
        c = (predicted == labels).squeeze()
        for i in range(4):
            label = labels[i]
            class_correct[label] += c[i].item()
            class_total[label] += 1


for i in range(10):
    print('Accuracy of %5s : %2d %%' % (
        classes[i], 100 * class_correct[i] / class_total[i]))

########################################################################
# Okay, so what next?
#
# How do we run these neural networks on the GPU?
#
# Training on GPU
# ----------------
# Just like how you transfer a Tensor on to the GPU, you transfer the neural
# net onto the GPU.
#
# Let's first define our device as the first visible cuda device if we have
# CUDA available:

#device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

# Assume that we are on a CUDA machine, then this should print a CUDA device:

#print(device)

########################################################################
# The rest of this section assumes that `device` is a CUDA device.
#
# Then these methods will recursively go over all modules and convert their
# parameters and buffers to CUDA tensors:
#
# .. code:: python
#
#     net.to(device)
#
#
# Remember that you will have to send the inputs and targets at every step
# to the GPU too:
#
# .. code:: python
#
#         inputs, labels = inputs.to(device), labels.to(device)
#
# Why dont I notice MASSIVE speedup compared to CPU? Because your network
# is realllly small.
#
# **Exercise:** Try increasing the width of your network (argument 2 of
# the first ``nn.Conv2d``, and argument 1 of the second ``nn.Conv2d`` –
# they need to be the same number), see what kind of speedup you get.
#
# **Goals achieved**:
#
# - Understanding PyTorch's Tensor library and neural networks at a high level.
# - Train a small neural network to classify images
#
# Training on multiple GPUs
# -------------------------
# If you want to see even more MASSIVE speedup using all of your GPUs,
# please check out :doc:`data_parallel_tutorial`.
#
# Where do I go next?
# -------------------
#
# -  :doc:`Train neural nets to play video games </intermediate/reinforcement_q_learning>`
# -  `Train a state-of-the-art ResNet network on imagenet`_
# -  `Train a face generator using Generative Adversarial Networks`_
# -  `Train a word-level language model using Recurrent LSTM networks`_
# -  `More examples`_
# -  `More tutorials`_
# -  `Discuss PyTorch on the Forums`_
# -  `Chat with other users on Slack`_
#
# .. _Train a state-of-the-art ResNet network on imagenet: https://github.com/pytorch/examples/tree/master/imagenet
# .. _Train a face generator using Generative Adversarial Networks: https://github.com/pytorch/examples/tree/master/dcgan
# .. _Train a word-level language model using Recurrent LSTM networks: https://github.com/pytorch/examples/tree/master/word_language_model
# .. _More examples: https://github.com/pytorch/examples
# .. _More tutorials: https://github.com/pytorch/tutorials
# .. _Discuss PyTorch on the Forums: https://discuss.pytorch.org/
# .. _Chat with other users on Slack: http://pytorch.slack.com/messages/beginner/
