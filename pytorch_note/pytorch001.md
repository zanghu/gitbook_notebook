# Autograd机制的原理

==================

This note will present an overview of how autograd works and records the operations. It's not strictly necessary to understand all this, but we recommend getting familiar with it, as it will help you write more efficient, cleaner programs, and can aid you in debugging.

本文将对autograd如何工作和如何记录运算进行综述。了解这一切严格来说并不是必须的，但是熟悉这部分内容将有助于你写出更为高效、干净的程序，并将对debugging有所助益。

## Excluding subgraphs from backward

Every Variable has two flags: `requires_grad` and `volatile`. They both allow for fine grained exclusion of subgraphs from gradient computation and can increase efficiency.

每个Variable对象都有两个作为标志位的属性：requires\_grad和volatile. 通过设置这两个属性的值可以使整个计算图的某些子图免于被计算梯度，同时提高效率。

### requires\_grad

If there's a single input to an operation that requires gradient, its output will also require gradient. Conversely, only if all inputs don't require gradient, the output also won't require it. Backward computation is never performed in the subgraphs, where all Variables didn't require gradients.

如果生成Varable的运算表达式至少有一个Varable输入的requies\_grad属性为True, 那么表达式输出的Variable对象的requires\_grad属性也将是True. 反之，只有当运算表达式的所有Varable输入的requires\_grad属性都为False, 输出Variable对象的requires\_grad属性才会是False. 如果一个子图的所有Variable的requires\_grad属性都是False, 那么Backward过程将不会在这个子图上引发任何计算。

```python
    >>> x = Variable(torch.randn(5, 5))
    >>> y = Variable(torch.randn(5, 5))
    >>> z = Variable(torch.randn(5, 5), requires_grad=True)
    >>> a = x + y
    >>> a.requires_grad
    False
    >>> b = a + z
    >>> b.requires_grad
    True
```

This is especially useful when you want to freeze part of your model, or you know in advance that you're not going to use gradients w.r.t. some parameters. For example if you want to finetune a pretrained CNN, it's enough to switch the `requires_grad` flags in the frozen base, and no intermediate buffers will be saved, until the computation gets to the last layer, where the affine transform will use weights that require gradient, and the output of the network will also require them.

特别是当你希望冻结你的模型的一部分时，或者你更进一步的知道你不会用到哪些参数的梯度时，这种方法（将对应子图的Variable的requires\_grad属性设为False）将会非常有效。举例来说，当你希望利用一个预训练好的CNN作为特征提取层来训练一个基于该CNN的Softmax层，那么就可以

```python
    model = torchvision.models.resnet18(pretrained=True)
    for param in model.parameters():
        param.requires_grad = False
    # Replace the last fully-connected layer
    # Parameters of newly constructed modules have requires_grad=True by default
    model.fc = nn.Linear(512, 100)

    # Optimize only the classifier
    optimizer = optim.SGD(model.fc.parameters(), lr=1e-2, momentum=0.9)
```

### volatile

Volatile is recommended for purely inference mode, when you're sure you won't be even calling `.backward()`. It's more efficient than any other autograd setting - it will use the absolute minimal amount of memory to evaluate the model. `volatile` also determines that `requires_grad` is False.

Volatile differs from `excluding-requires_grad` in how the flag propagates. If there's even a single volatile input to an operation, its output is also going to be volatile. Volatility spreads across the graph much easier than non-requiring gradient - you only need a **single** volatile leaf to have a volatile output, while you need **all** leaves to not require gradient to have an output that doesn't require gradient. Using volatile flag you don't need to change any settings of your model parameters to use it for inference. It's enough to create a volatile input, and this will ensure that no intermediate states are saved.

```python
>>> regular\_input = Variable\(torch.randn\(1, 3, 227, 227\)\)  
>>> volatile\_input = Variable\(torch.randn\(1, 3, 227, 227\), volatile=True\)  
>>> model = torchvision.models.resnet18\(pretrained=True\)  
>>> model\(regular\_input\).requires\_grad  
>>>     True  
>>> model\(volatile\_input\).requires\_grad  
>>>     False  
>>> model\(volatile\_input\).volatile  
>>>     True  
>>> model\(volatile\_input\).grad\_fn is None  
>>>     True
```

How autograd encodes the history  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Autograd is reverse automatic differentiation system.  Conceptually,  
autograd records a graph recording all of the operations that created  
the data as you execute operations, giving you a directed acyclic graph  
whose leaves are the input variables and roots are the output variables.  
By tracing this graph from roots to leaves, you can automatically  
compute the gradients using the chain rule.

Internally, autograd represents this graph as a graph of  
:class:`Function` objects \(really expressions\), which can be  
:meth:`~torch.autograd.Function.apply` ed to compute the result of  
evaluating the graph.  When computing the forwards pass, autograd  
simultaneously performs the requested computations and builds up a graph  
representing the function that computes the gradient \(the `.grad_fn`  
attribute of each :class:`Variable` is an entry point into this graph\).  
When the forwards pass is completed, we evaluate this graph in the  
backwards pass to compute the gradients.

An important thing to note is that the graph is recreated from scratch at every  
iteration, and this is exactly what allows for using arbitrary Python control  
flow statements, that can change the overall shape and size of the graph at  
every iteration. You don't have to encode all possible paths before you  
launch the training - what you run is what you differentiate.

In-place operations on Variables  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Supporting in-place operations in autograd is a hard matter, and we discourage  
their use in most cases. Autograd's aggressive buffer freeing and reuse makes  
it very efficient and there are very few occasions when in-place operations  
actually lower memory usage by any significant amount. Unless you're operating  
under heavy memory pressure, you might never need to use them.

There are two main reasons that limit the applicability of in-place operations:

1. Overwriting values required to compute gradients. This is why variables don't  
   support `log_`. Its gradient formula requires the original input, and while  
   it is possible to recreate it by computing the inverse operation, it is  
   numerically unstable, and requires additional work that often defeats the  
   purpose of using these functions.

2. Every in-place operation actually requires the implementation to rewrite the  
   computational graph. Out-of-place versions simply allocate new objects and  
   keep references to the old graph, while in-place operations, require  
   changing the creator of all inputs to the :class:`Function` representing  
   this operation. This can be tricky, especially if there are many Variables  
   that reference the same storage \(e.g. created by indexing or transposing\),  
   and in-place functions will actually raise an error if the storage of  
   modified inputs is referenced by any other :class:`Variable`.

In-place correctness checks  
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Every variable keeps a version counter, that is incremented every time it's  
marked dirty in any operation. When a Function saves any tensors for backward,  
a version counter of their containing Variable is saved as well. Once you access  
`self.saved_tensors` it is checked, and if it's greater than the saved value  
an error is raised.

