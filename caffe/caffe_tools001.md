## 谷歌命令行参数处理工具——gflags

gflags是一种用来在代码中简化处理程序启动时命令行输入参数的工具库。

#### 1.官方文档

* 官方文档网站：[https://gflags.github.io/gflags/](https://gflags.github.io/gflags/)

内容不多，介绍的也不太全面，整体印象较差。

* 推荐文档：gflags的头文件gflags.h，其中每个接口都有详细的功能说明。

#### 2.示例代码

用一个代码例子来说明gflags的用法：

* build.sh

```shell
#!/bin/sh
set -ex
g++ -g -Wall test.cpp -lgflags -o Test
```

* test.cpp

```c
/* test.cpp */
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

#include <gflags/gflags.h>

DEFINE_string(gpu, "",
    "Optional; run in GPU mode on given device IDs separated by ','."
    "Use '-gpu all' to run on all available GPUs. The effective training "
    "batch size is multiplied by the number of devices.");
DEFINE_string(solver, "",
    "The solver definition protocol buffer text file.");
DEFINE_string(model, "",
    "The model definition protocol buffer text file.");
DEFINE_string(phase, "",
    "Optional; network phase (TRAIN or TEST). Only used for 'time'.");
DEFINE_int32(level, 0,
    "Optional; network level.");
DEFINE_string(stage, "",
    "Optional; network stages (not to be confused with phase), "
    "separated by ','.");
DEFINE_string(snapshot, "",
    "Optional; the snapshot solver state to resume training.");
DEFINE_string(weights, "",
    "Optional; the pretrained weights to initialize finetuning, "
    "separated by ','. Cannot be set simultaneously with snapshot.");
DEFINE_int32(iterations, 50,
    "The number of iterations to run.");
DEFINE_string(sigint_effect, "stop",
             "Optional; action to take when a SIGINT signal is received: "
              "snapshot, stop or none.");
DEFINE_string(sighup_effect, "snapshot",
             "Optional; action to take when a SIGHUP signal is received: "
             "snapshot, stop or none.");

/**
 * 编译出的可执行文件是Test，运行命令如下：
 * ./Test -gpu=cuda -solver=sgd -model=cnn -phase=train -level=10 -stage=unknow -snapshot=snapshot -weights=weights -iterations=50 -sigint_effect=yes -sighup_effect=yes
 */
int main(int argc, char **argv)
{
    ::google::SetVersionString("1.0.0"); // ./Test --version 查看版本信息
    ::google::SetUsageMessage("Usage : ./demo "); // ./Test --help 查看用法
    
    ::google::ParseCommandLineFlags(&argc, &argv, true); // 初始化gflags

    std::cout << "gpu: " << FLAGS_gpu << std::endl; // string
    std::cout << "solver: " << FLAGS_solver << std::endl; // string
    std::cout << "model: " << FLAGS_model << std::endl; // string
    std::cout << "phase: " << FLAGS_phase << std::endl; // string
    std::cout << "level: " << FLAGS_level << std::endl; // int32
    std::cout << "stage: " << FLAGS_stage << std::endl; // string
    std::cout << "snapshot: " << FLAGS_snapshot << std::endl; // string
    std::cout << "weights: " << FLAGS_weights << std::endl; // string
    std::cout << "iterations: " << FLAGS_iterations << std::endl; // int32
    std::cout << "sigint_effect: " << FLAGS_sigint_effect << std::endl; // string
    std::cout << "sighup_effct: " << FLAGS_sighup_effect << std::endl; // string

    ::google::ShutDownCommandLineFlags(); // 释放gflags占用资源, 执行后用valgrind检测确认没有资源泄露

    std::cout << "finish" << std::endl;
    return 0;
}
```

* 运行结果：

![](/assets/google001_001.PNG)

