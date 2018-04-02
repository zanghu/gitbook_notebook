## Maven常用命令

参考资料：[https://www.cnblogs.com/asingna/p/5234908.html](https://www.cnblogs.com/wkrbky/p/6352188.html)

  
Maven库：

[http://repo2.maven.org/maven2/](http://repo2.maven.org/maven2/)

Maven依赖查询：

[http://mvnrepository.com/](http://mvnrepository.com/)

一，Maven常用命令：  
1. 创建Maven的普通Java项目：

mvn archetype:create  
    -DgroupId=packageName  
    -DartifactId=projectName  
2. 创建Maven的Web项目：

mvn archetype:create  
    -DgroupId=packageName  
    -DartifactId=webappName  
    -DarchetypeArtifactId=maven-archetype-webapp  
3. 反向生成 maven 项目的骨架：

mvn archetype:generate  
　　你是怎么创建你的maven项目的?是不是像这样:

mvn archetype:create -DarchetypeArtifactId=maven-archetype-quickstart -DgroupId=com.ryanote -Dartifact=common  
　　如果你还再用的话,那你就out了,现代人都用mvn archetype:generate了,它将创建项目这件枯燥的事更加人性化,你再也不需要记那么多的archetypeArtifactId,你只需输入archetype:generate,剩下的就是做”选择题”了.

cmd步骤：

缩写写法：

mvn archetype:generate -DgroupId=otowa.user.dao -DartifactId=user-dao -Dversion=0.01-SNAPSHOT  
4. 编译源代码：

mvn compile  
5. 编译测试代码：

mvn test-compile  
6. 运行测试:

mvn test  
7. 产生site：

mvn site  
8. 打包：

mvn package  
9. 在本地Repository中安装jar：

mvn install  
例：installing D:\xxx\xx.jar to D:\xx\xxxx  
10. 清除产生的项目：

mvn clean  
11. 生成eclipse项目：

mvn eclipse:eclipse  
12. 生成idea项目：

mvn idea:idea  
13. 组合使用goal命令，如只打包不测试：

mvn -Dtest package  
14. 编译测试的内容：

mvn test-compile  
15. 只打jar包:

mvn jar:jar  
16. 只测试而不编译，也不测试编译：

mvn test -skipping compile -skipping test-compile  
 \( -skipping 的灵活运用，当然也可以用于其他组合命令\)  
17. 清除eclipse的一些系统设置:

mvn eclipse:clean  
18.查看当前项目已被解析的依赖：

mvn dependency:list  
19.上传到私服：

mvn deploy  
20. 强制检查更新，由于快照版本的更新策略\(一天更新几次、隔段时间更新一次\)存在，如果想强制更新就会用到此命令:

mvn clean install-U  
21. 源码打包：

mvn source:jar  
或  
mvn source:jar-no-fork  
mvn compile与mvn install、mvn deploy的区别  
mvn compile，编译类文件  
mvn install，包含mvn compile，mvn package，然后上传到本地仓库  
mvn deploy,包含mvn install,然后，上传到私服

