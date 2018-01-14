## \[Pytorch\]

### torch.Tensor的squeeze\(\)方法与unsqueeze\(\)方法

官方文档中的解释：

```python
torch.unsqueeze(input, dim, out=None)
```

Returns a new tensor with a dimension of size one inserted at the specified position.  
The returned tensor shares the same underlying data with this tensor.  
A negative dim value can be used and will correspond todim+input.dim\(\)+1dim+input.dim\(\)+1

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



