### Pytorch: 检查cudnn版本

#### 1.调用方法
`pytorch`提供了官方的`cudnn`版本检查接口，调用方法如下：

```python
>>> import torch
>>> torch.backends.cudnn.version()
7605
```


#### 2.含义说明
`7`表示MAJOR版本号为7
`6`表示MINOR版本号为6
`05`表示PATCH版本号为5
即目前`pytorch`使用的`cudnn`是`7.6.5`版

#### 3.原理分析

`torch.backends.cudnn.version()`方法内置了编译期和运行期`cudnn`版本校验（比较`pytorch`编译期依赖的`cudnn`版本和用户本地运行期`cudnn`版本是否一致）

**编译期版本获取原理：**
`pytorch`在编译期将当时包含的`cudnn.h`文件中的版本号作为字面常量写死在代码的`getCompileVersion`函数中，如下：

```cuda
version_tuple getCompileVersion() {
  return version_tuple(CUDNN_MAJOR, CUDNN_MINOR, CUDNN_PATCHLEVEL);
}
```

**运行期版本获取原理：**
`pytorch`的`getRuntimeVersion`函数，在运行期通过用户本地`cudnn.h`文件中提供的`cudnnGetVersion()`方法声明，调用运行期链接的`cudnn`动态库中的方法，获得运行期版本号。

```cuda
version_tuple getRuntimeVersion() {
  auto version = cudnnGetVersion(); // cudnnGetVersion()是cudnn.h中的声明
  auto major = version / 1000;
  auto minor = (version % 1000) / 100;
  auto patch = version % 10;
  return version_tuple(major, minor, patch);
}
```

如果上面的`getCompileVersion`方法和`getRuntimeVersion`方法返回的版本不满足兼容性条件（例如：主版本号不一致等），则`torch.backends.cudnn.version()`会直接抛出异常。
