## Maven常用命令

参考资料：[https://www.cnblogs.com/asingna/p/5234908.html](https://www.cnblogs.com/wkrbky/p/6352188.html)

  
Maven库：

[http://repo2.maven.org/maven2/](http://repo2.maven.org/maven2/)

Maven依赖查询：

[http://mvnrepository.com/](http://mvnrepository.com/)

### 一，Maven常用命令：  
#### 1. 创建Maven的普通Java项目：
目前"archetype:create"经被"archetype:generate"所取代，用来创建maven项目
```shell
mvn archetype:generate -DgroupId=otowa.user.dao -DartifactId=user-dao -Dversion=0.01-SNAPSHOT
```
#### 2. 创建Maven的Web项目：
```shell
mvn archetype:create  
    -DgroupId=packageName  
    -DartifactId=webappName  
    -DarchetypeArtifactId=maven-archetype-webapp
```  
#### 3. 创建 maven 项目
目前"archetype:create"经被"archetype:generate"所取代，用来创建maven项目
```shell
mvn archetype:generate -DgroupId=otowa.user.dao -DartifactId=user-dao -Dversion=0.01-SNAPSHOT
```
#### 4. 编译源代码：
```shell
mvn compile
```
#### 5. 编译测试代码：
```shell
mvn test-compile 
```
#### 6. 运行测试:
```shell
mvn test  
```
#### 7. 产生site：
```shell
mvn site  
```
#### 8. 打包：
```shell
mvn package
```  
#### 9. 在本地Repository中安装jar：
```shell
mvn install  
```
例：installing D:\xxx\xx.jar to D:\xx\xxxx  
#### 10. 清除产生的项目：
```shell
mvn clean
```  
#### 11. 生成eclipse项目：
```shell
mvn eclipse:eclipse
```  
#### 12. 生成idea项目：
```shell
mvn idea:idea
```  
#### 13. 组合使用goal命令，如只打包不测试：
```shell
mvn -Dtest package 
``` 
#### 14. 编译测试的内容：
```shell
mvn test-compile
```  
#### 15. 只打jar包:
```shell
mvn jar:jar  
```
#### 16. 只测试而不编译，也不测试编译：
```shell
mvn test -skipping compile -skipping test-compile  
```
 \( -skipping 的灵活运用，当然也可以用于其他组合命令\)  
#### 17. 清除eclipse的一些系统设置:
```shell
mvn eclipse:clean  
```
#### 18.查看当前项目已被解析的依赖：
```shell
mvn dependency:list
```  
#### 19.上传到私服：
```shell
mvn deploy
```  
#### 20. 强制检查更新，由于快照版本的更新策略\(一天更新几次、隔段时间更新一次\)存在，如果想强制更新就会用到此命令:
```shell
mvn clean install-U
```
#### 21. 源码打包：
```shell
mvn source:jar  
#或  
mvn source:jar-no-fork  
```

#### mvn compile与mvn install、mvn deploy的区别  
mvn compile，编译类文件  
mvn install，包含mvn compile，mvn package，然后上传到本地仓库  
mvn deploy,包含mvn install,然后，上传到私服

