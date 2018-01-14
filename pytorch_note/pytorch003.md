## \[Pytorch\]

### torch.Tensor的squeeze\(\)方法与unsqueeze\(\)方法

#### 1.**torch.unsqueeze**

功能说明：向torch.Tensor或torch.autograd.Variable对象中增加一个轴, 新增轴维数为1

官方文档中的解释：

```python
torch.unsqueeze(input, dim, out=None)
```

Returns a new tensor with a dimension of size one inserted at the specified position.  
The returned tensor shares the same underlying data with this tensor.  
A negative dim value can be used and will correspond todim+input.dim\(\)+1dim+input.dim\(\)+1

**Parameters:**
    input (Tensor) – the input tensor
    dim (int) – the index at which to insert the singleton dimension
    out (Tensor, optional) – the output tensor

**Example:**

```python
>>> x = torch.Tensor([1, 2, 3, 4])
>>> torch.unsqueeze(x, 0)
 1  2  3  4
[torch.FloatTensor of size 1x4]
>>> torch.unsqueeze(x, 1)
 1
 2
 3
 4
[torch.FloatTensor of size 4x1]
```

#### 2.**torch.squeeze**

功能说明：去掉torch.Tensor或torch.autograd.Variable对象中维数为1的轴向

```python
torch.squeeze(input, dim=None, out=None)
```

官方文档中的解释：  
Returns a tensor with all the dimensions of input of size 1 removed.  
For example, if input is of shape: \(A×1×B×C×1×D\)\(A×1×B×C×1×D\) then the out tensor will be of shape: \(A×B×C×D\)\(A×B×C×D\).  
When dim is given, a squeeze operation is done only in the given dimension. If input is of shape: \(A×1×B\)\(A×1×B\), squeeze\(input, 0\) leaves the tensor unchanged, but squeeze\(input, 1\) will squeeze the tensor to the shape \(A×B\)\(A×B\)


**Parameters:**	
input (Tensor) – the input tensor
dim (int, optional) – if given, the input will be squeezed only in this dimension
out (Tensor, optional) – the output tensor
**Example:**
```python
>>> x = torch.zeros(2,1,2,1,2)
>>> x.size()
(2L, 1L, 2L, 1L, 2L)
>>> y = torch.squeeze(x)
>>> y.size()
(2L, 2L, 2L)
>>> y = torch.squeeze(x, 0)
>>> y.size()
(2L, 1L, 2L, 1L, 2L)
>>> y = torch.squeeze(x, 1)
>>> y.size()
(2L, 2L, 1L, 2L)
```
**注意**
a.\)As an exception to the above, a 1-dimensional tensor of size 1 will not have its dimensions changed
b.\)The returned tensor shares the storage with the input tensor, so changing the contents of one will change the contents of the other.