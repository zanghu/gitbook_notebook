## Jupyter: 自定义配置

参考资料: [清华大学Pypi镜像](https://mirror.tuna.tsinghua.edu.cn/help/pypi/)

参考资料：[Pypi官方网站](https://pypi.org/)

### 1.自动换行和行号

* 首先通过命令`jupyter --config-dir`找到配置文件所在路径, 进入下一级路径 nbconfig 
![](/assets/python049_01.png)

* 找到`notebook.json 文件`(如果没有, 自行创建即可), 通过 notepad++ 打开文件进行编辑

* 追加写入如下代码, 保存并重新启动`Jupyter Notebook`

```json
{
  "Cell": {
    "cm_config": {
      "lineNumbers": true,
      "lineWrapping": true
    }
  },
  "MarkdownCell": {
    "cm_config": {
      "lineWrapping": true
    }
  },
  "CodeCell": {
    "cm_config": {
      "lineWrapping": true
    }
  }
}
```


