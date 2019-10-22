### 从字符串拼接路径的正确方式

参考资料：[How to combine paths in Java?](https://stackoverflow.com/questions/412380/how-to-combine-paths-in-java)

#### 1.路径组装

If you're using Java 7 or Java 8, you should strongly consider using `java.nio.file.Path` and `java.nio.file.Paths`. For example:

```java
import java.nio.file.Path;
import java.nio.file.Paths;

...
Path path = Paths.get("/home/", "/zanghu", "baz.txt");
String pathStr = path.toString();
...
```

`Path.resolve` 可以用于将指定路径与当前所在路径拼接。

```java
public void usePath() {
    Path path1 = Paths.get("folder1", "sub1");
    Path path2 = Paths.get("folder2", "sub2");
    path1.resolve(path2); //folder1\sub1\folder2\sub2
    path1.resolveSibling(path2); //folder1\folder2\sub2
    path1.relativize(path2); //..\..\folder2\sub2
    path1.subpath(0, 1); //folder1
    path1.startsWith(path2); //false
    path1.endsWith(path2); //false
    Paths.get("folder1/./../folder2/my.text").normalize(); //folder2\my.text
}
```