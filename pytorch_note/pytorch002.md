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

as view is only supposed to work on contiguous tensors, and transposing a tensor makes it non-contiguous. You can use .contiguous\(\) after transpose to fix your issue

