### 从字符串拼接路径的正确方式

参考资料：[CSDN: Java的三种代理模式](https://www.cnblogs.com/qlqwjy/p/7550609.html)

参考资料: [知乎：一文带你入门面向切面编程（AOP）](https://zhuanlan.zhihu.com/p/67577468)

#### 1.面向切面编程

>参考：[知乎：一文带你入门面向切面编程（AOP）](https://zhuanlan.zhihu.com/p/67577468)

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

* **实现方式**

其实AOP只是一种编程范式，要用什么方式去实现并没有明确的要求，就像OOP并不一定需要Java来实现一样。实现AOP的关键是如何将上面的时间计算代码织入work方法中，目前业内主要有两种织入方式，一种是在编译期、编译后、类加载期织入，其代表性框架是AspectJ。一种是通过在运行时生成**代理对象**来织入的，Spring AOP就是通过这种方式实现的。

#### 2.Java中的代理模式

##### 2.1.代理模式
代理(Proxy)是一种设计模式,提供了对目标对象另外的访问方式;即通过代理对象访问目标对象.这样做的好处是:可以在目标对象实现的基础上,增强额外的功能操作,即扩展目标对象的功能.
这里使用到编程中的一个思想:不要随意去修改别人已经写好的代码或者方法,如果需改修改,可以通过代理的方式来扩展该方法

举个例子来说明代理的作用:假设我们想邀请一位明星,那么并不是直接连接明星,而是联系明星的经纪人,来达到同样的目的.明星就是一个目标对象,他只要负责活动中的节目,而其他琐碎的事情就交给他的代理人(经纪人)来解决.这就是代理思想在现实中的一个例子

用图表示如下:

![](/assets/java007_01.png)

