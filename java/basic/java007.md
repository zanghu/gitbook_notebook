### 从字符串拼接路径的正确方式

参考资料：[CSDN: Java的三种代理模式](https://www.cnblogs.com/qlqwjy/p/7550609.html)

参考资料: [知乎：一文带你入门面向切面编程（AOP）](https://zhuanlan.zhihu.com/p/67577468)

#### 1.面向切面编程

>参考：[知乎：一文带你入门面向切面编程（AOP）](https://zhuanlan.zhihu.com/p/67577468)

#### 1.1.何为AOP

* **定义**

`AOP`(Aspect Oriented Programming）中文翻译为面向切面编程，是指通过预编译方式和运行期动态代理实现程序中某些功能的统一维护的一种技术。

这门技术有什么好处呢？自然是降低程序各个部分之间的耦合度。

* **目的**

理论上程序中任何公共处理的逻辑，都可以独立出来用AOP实现，一般在工程实际中用AOP是为了实现系统级服务与应用的业务逻辑之间的彻底分离。这样一来，就不用在业务代码中插满各种乱七八糟的系统分析代码了。好了，这么说还是很抽象，还是看个例子吧。

* **举个例子**

假如你有一个奴隶接口，他有一个work方法帮你干某种累人的工作

```java
public interface Slaver {
    void work();
}
```

下面是这个接口的一个具体的实现，也就是你要它干的一项工作

```java
@Component
public class SlaverImp implements Slaver {
    public void work() {
        System.out.println("I'm working hard......");
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }
}
```

现在你想知道这个奴隶完成这个工作需要花费多长时间，但你又不想影响它工作（不在work方法里面添加代码）该怎么办？

废话少说了，这正是AOP大显身手的地方了，你完全不需要触碰业务代码，就可以对服务进行一些系统操作。怎么实现了？首先用@Aspect注解声明一个切面（Aspect）类：

```java
@Aspect
public class SlaverAspect {
    @Pointcut("execution(public * service.SlaverImp.work())")
    public void recordStatus(){}
}
```

然后再声明一个增强（Advice）类，在增强类里面加上你需要的时间计算逻辑：

```java
@Aspect
@Component
public class SlaverAdvice {
    @Around("aop.aspect.SlaverAspect.recordStatus()" )
    public void recordTimeConsuming(ProceedingJoinPoint joinPoint) throws Throwable{
        long startTime = System.currentTimeMillis();
        joinPoint.proceed();
        long endTime = System.currentTimeMillis();
        System.out.println(String.format("Time consumed: %s", endTime-startTime));
    }
}
```

这样，调用方在调用SlaverImp的work方法时，不仅会执行原有的业务逻辑，也会执行你的时间计算逻辑。神奇吧，这是如何实现的呢？下面将会揭晓。

* **技术实现**