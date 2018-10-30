## 谷歌工具——gflags

gflags是一种用来在代码中简化处理程序启动时命令行输入参数的工具库。

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
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "finish" << std::endl;

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

    ::google::ShutDownCommandLineFlags();

    return 0;
}
```