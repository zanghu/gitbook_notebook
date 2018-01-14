## \[Pytorch\]

### torch.Tensor的squeeze\(\)方法与unsqueeze\(\)方法

官方文档中的解释：

torch.unsqueeze(input, dim, out=None)

Returns a new tensor with a dimension of size one inserted at the specified position.

The returned tensor shares the same underlying data with this tensor.

A negative dim value can be used and will correspond todim+input.dim\(\)+1dim+input.dim\(\)+1

| Parameters: | **input**\([_Tensor_](http://pytorch.org/docs/master/tensors.html#torch.Tensor)\) – the input tensor**dim**\([_int_](https://docs.python.org/2/library/functions.html#int)\) – the index at which to insert the singleton dimension**out**\([_Tensor_](http://pytorch.org/docs/master/tensors.html#torch.Tensor)_,optional_\) – the output tensor |
| :--- | :--- |




