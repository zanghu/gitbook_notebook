##安装conda包

有关在Anaconda Prompt（Linux或macOS上的终端）中使用conda包管理器的更多信息，请参阅conda文档。

您还可以使用图形界面 Anaconda Navigator只需点击几下即可安装conda软件包。

打开Anaconda提示符（Linux或macOS上的终端）并按照这些说明操作。

### 安装conda包
输入命令：

```shell
$ conda install package-name
```
注意：替换`package-name`为您的包裹名称。

### 安装特定版本的conda软件包
在包名称后面包含所需的版本号或其前缀：

```shell
$ conda install package-name=2.3.4
```
> 注意：替换`package-name`为您的包裹名称。替换 `2.3.4`为所需的版本号。

要仅指定主要版本，请运行：

```shell
$ conda install package-name=2
```
> 注意：替换`package-name`为您的包裹名称。替换 `2`为所需的版本号。

这些命令将安装到当前处于活动状态的环境中。要安装到命名环境中，请运行：

```shell
$ conda install package-name=2.3.4 -n some-environment
```
> 注意：替换`package-name`为您的包裹名称。替换 `2.3.4`为所需的版本号。替换 `some-environment`为您的环境名称。

如果包特定于Python版本，则conda使用当前或命名环境中安装的版本。有关版本，依赖关系和通道的详细信息，请参阅 Conda常见问题解答和 Conda疑难解答。

### 在非联网计算机上安装软件包
要从本地计算机直接安装conda软件包，请运行：

```shell
$ conda install /package-path/package-filename.tar.bz2
```
> 注意：替换`package-path`和`package-filename.tar.bz2`使用您的实际路径和文件名。

Conda将软件包安装到`anaconda/pkgs`目录中。

要安装包含许多conda包的.tar文件，请运行以下命令：

```shell
$ conda install /packages-path/packages-filename.tar
```
> 注意：替换`package-path`和`package-filename.tar`使用您的实际路径和文件名。

如果conda找不到该文件，请尝试使用绝对路径名而不是相对路径名。

> 注意：直接从文件安装包不会解决依赖性。如果您安装的软件包不起作用，则可能缺少需要手动解决的依赖项。