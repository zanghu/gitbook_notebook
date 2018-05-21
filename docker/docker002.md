## docker-compose 学习

参考资料1：[https://docs.docker.com/compose/overview/](https://docs.docker.com/compose/overview/)

参考资料2：[https://docs.docker.com/compose/gettingstarted/](https://docs.docker.com/compose/gettingstarted/)

### 1.使用 dokcer-compose 的三个基本步骤：

* 在 DockerFile 文件中定义你的应用的环境变量，这样应用即可在任何地方被复现. 

* 在 docker-compose.yml 文件中定义构成你的应用的服务，这样它们就可以在一个孤立环境上一起运行. 

* 执行 docker-compose up 命令，之后 Compose 就会启动和运行你的整个应用. 

