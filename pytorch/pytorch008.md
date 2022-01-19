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

该方法内置了编译期和运行期`cudnn`版本校验（比较`pytorch`编译期依赖的`cudnn`版本和用户本地运行期`cudnn`版本是否一致）

`pytorch`在编译期将当时包含的`cudnn.h`文件中的版本号作为字面常量写死在代码的`getCompileVersion`函数中，如下：

```cuda
version_tuple getCompileVersion() {
  return version_tuple(CUDNN_MAJOR, CUDNN_MINOR, CUDNN_PATCHLEVEL);
}
```