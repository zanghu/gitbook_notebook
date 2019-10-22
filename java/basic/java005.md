### 从字符串拼接路径的正确方式

参考资料：[How to combine paths in Java?](https://stackoverflow.com/questions/412380/how-to-combine-paths-in-java)


If you're using Java 7 or Java 8, you should strongly consider using `java.nio.file.Path` and `java.nio.file.Paths`. For example:

```java
import java.nio.file.Path;
import java.nio.file.Paths;

...
Path path = Paths.get("/home/", "/zanghu", "baz.txt");
String pathStr = path.toString();
...
```

`Path.resolve` can be used to combine one path with another, or with a string. The Paths helper class is useful too. 
