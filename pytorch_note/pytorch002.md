# \[Pytorch\]torch.Tensor的permute\(\)方法与transpose方法的区别

### 1.transpose与permute的区别

transpose和permute都是用来对torch.Tensor做轴置换的，功能类似于numpy.transpose\(\)或theano.dimshuffle\(\)

Pytorch中transpose和permute二者功能的区别是：

transpose only applies to 2 axis, while permute can be applied to all the axes at the same time.  
For example

```python
a = torch.rand(1,2,3,4)
print(a.transpose(0,3).transpose(1,2).size())
print(a.permute(3,2,1,0).size())
```

BTW, permute internally calls transpose a number of times

参考网址: [https://discuss.pytorch.org/t/swap-axes-in-pytorch/970](https://discuss.pytorch.org/t/swap-axes-in-pytorch/970)

### 2.关于torch.Tensor的contiguous方法的用途

torch.Tensor对象使用transpose或permute返回的tensor，一般需要调用contiguous方法来使内存连续，才能正确调用view\(\)，原因是:

有些tensor不是一整块内存，而是又不同的数据块组成，而tensor的view\(\)操作依赖于内存是整块的，这时只需要执行以下contiguous\(\)这个操作。

as view is only supposed to work on contiguous tensors, and transposing a tensor makes it non-contiguous. You can use .contiguous\(\) after transpose to fix your issue

当不确定一个tensor是否是contiguous的时候, 可以用is\_contiguous\(\)方法来查看

```python
>>>import torch
>>>x = torch.ones(2, 2)
>>>x.is_contiguous()
True
>>>y = x.transpose(0, 1)
>>>y.is_contiguous()
False
>>>z = y.contiguous()
>>>z.is_contiguous()
True
```

### 3.对于tensor.autograd.Variable

tensor.autograd.Variable对象可以直接调用上面提到的所有方法, 包括: transpose, permute, contiguous, is\_contiguous, 用法与torch.Tensor相同

```python
>>>import torch
>>>a = torch.autograd.Variable(torch.ones(2, 2))
>>>a.is_contiguous()
True
>>>b = a.transpose(0, 1) # 即使没有发生实际的轴置换, 返回结果仍然会变成非contiguous的
>>>b.__class__
<class 'torch.autograd.variable.Variable'>
>>>b.is_contiguous()
False
>>>c = b.contiguous()
>>>c.is_contiguous()
True
```



