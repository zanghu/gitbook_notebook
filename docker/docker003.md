## docker的一般概念与注意事项

### 1.关于Dockerfile

* Step 1: 在 DockerFile 文件中定义你的应用的环境变量，这样应用即可在任何地方被复现.

* Step 2: 在 docker-compose.yml 文件中定义构成你的应用的服务，这样它们就可以在一个孤立环境上一起运行.

* Step 3: 执行 docker-compose up 命令，之后 Compose 就会启动和运行你的整个应用.



