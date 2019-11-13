### 从字符串拼接路径的正确方式

参考资料：[CSDN: Java的三种代理模式](https://www.cnblogs.com/qlqwjy/p/7550609.html)

参考资料: [知乎：一文带你入门面向切面编程（AOP）](https://zhuanlan.zhihu.com/p/67577468)

参考资料：《疯狂JAVA讲义（第二版）》

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

>参考：[CSDN: Java的三种代理模式](https://www.cnblogs.com/qlqwjy/p/7550609.html)

##### 2.1.代理模式
代理(Proxy)是一种设计模式,提供了对目标对象另外的访问方式;即通过代理对象访问目标对象.这样做的好处是:可以在目标对象实现的基础上,增强额外的功能操作,即扩展目标对象的功能.
这里使用到编程中的一个思想:不要随意去修改别人已经写好的代码或者方法,如果需改修改,可以通过代理的方式来扩展该方法

举个例子来说明代理的作用:假设我们想邀请一位明星,那么并不是直接连接明星,而是联系明星的经纪人,来达到同样的目的.明星就是一个目标对象,他只要负责活动中的节目,而其他琐碎的事情就交给他的代理人(经纪人)来解决.这就是代理思想在现实中的一个例子

用图表示如下:

![](/assets/java007_01.png)

代理模式的关键点是:代理对象与目标对象.代理对象是对目标对象的扩展,并会调用目标对象

##### 2.2.静态代理(类似于装饰者模式)

**定义**
静态代理在使用时,需要定义接口或者父类,被代理对象与代理对象一起实现相同的接口或者是继承相同父类.

**案例**

模拟保存动作,定义一个保存动作的接口:IUserDao.java,然后目标对象实现这个接口的方法UserDao.java,此时如果使用静态代理方 式,就需要在代理对象(UserDaoProxy.java)中也实现IUserDao接口.调用的时候通过调用代理对象的方法来调用目标对象.
需要注意的是,代理对象与目标对象要实现相同的接口,然后通过调用相同的方法来调用目标对象的方法

接口:IUserDao.java
```java
复制代码
/**
 * 接口
 */
public interface IUserDao {

    void save();
}
```
 

 
目标对象:UserDao.java
```java
复制代码
/**
 * 接口实现
 * 目标对象
 */
public class UserDao implements IUserDao {
    public void save() {
        System.out.println("----已经保存数据!----");
    }
}
```

代理对象:UserDaoProxy.java
```java
/**
 * 代理对象,静态代理
 */
public class UserDaoProxy implements IUserDao{
    //接收保存目标对象
    private IUserDao target;
    public UserDaoProxy(IUserDao target){
        this.target=target;
    }

    public void save() {
        System.out.println("开始事务...");
        target.save();//执行目标对象的方法
        System.out.println("提交事务...");
    }
}
```
 
测试类:App.java
```
/**
 * 测试类
 */
public class App {
    public static void main(String[] args) {
        //目标对象
        UserDao target = new UserDao();

        //代理对象,把目标对象传给代理对象,建立代理关系
        UserDaoProxy proxy = new UserDaoProxy(target);

        proxy.save();//执行的是代理的方法
    }
}
```
 
**静态代理总结**

（1）可以做到在不修改目标对象的功能前提下,对目标功能扩展.
（2）缺点:因为代理对象需要与目标对象实现一样的接口,所以会有很多代理类,类太多.同时,一旦接口增加方法,目标对象与代理对象都要维护.
如何解决静态代理中的缺点呢?答案是可以使用动态代理方式

##### 2.3.动态代理

**特点**

（1）代理对象,不需要实现接口
（2）代理对象的生成,是利用JDK的API,动态的在内存中构建代理对象(需要我们指定创建代理对象/目标对象实现的接口的类型)
（3）动态代理也叫做:JDK代理,接口代理

JDK中生成代理对象的API
代理类所在包:`java.lang.reflect.Proxy`
JDK实现代理只需要使用newProxyInstance方法,但是该方法需要接收三个参数,完整的写法是:

```java
static Object newProxyInstance(ClassLoader loader, Class<?>[] interfaces,InvocationHandler h )
```

注意该方法是在Proxy类中是静态方法,且接收的三个参数依次为:

`ClassLoader loader`:指定当前目标对象使用类加载器,获取加载器的方法是固定的
`Class<?>[] interfaces`:目标对象实现的接口的类型,使用泛型方式确认类型
`InvocationHandler h`:事件处理,执行目标对象的方法时,会触发事件处理器的方法,会把当前执行目标对象的方法作为参数传入

**代码示例**

接口类IUserDao.java以及接口实现类,目标对象UserDao是一样的,没有做修改.在这个基础上,增加一个代理工厂类 (ProxyFactory.java),将代理类写在这个地方,然后在测试类(需要使用到代理的代码)中先建立目标对象和代理对象的联系,然后代用代理 对象的中同名方法

代理工厂类:ProxyFactory.java
```java
/**
 * 创建动态代理对象
 * 动态代理不需要实现接口,但是需要指定接口类型
 */
public class ProxyFactory{

    //维护一个目标对象
    private Object target;
    public ProxyFactory(Object target){
        this.target=target;
    }

   //给目标对象生成代理对象
    public Object getProxyInstance(){
        return Proxy.newProxyInstance(
                target.getClass().getClassLoader(),
                target.getClass().getInterfaces(),
                new InvocationHandler() {
                    @Override
                    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
                        System.out.println("开始事务2");
                        //运用反射执行目标对象方法
                        Object returnValue = method.invoke(target, args);
                        System.out.println("提交事务2");
                        return returnValue;
                    }
                }
        );
    }

}
```

测试类:App.java
```java
/**
 * 测试类
 */
public class App {
    public static void main(String[] args) {
        // 目标对象
        IUserDao target = new UserDao();
        // 【原始的类型 class cn.itcast.b_dynamic.UserDao】
        System.out.println(target.getClass());

        // 给目标对象，创建代理对象
        IUserDao proxy = (IUserDao) new ProxyFactory(target).getProxyInstance();
        // class $Proxy0   内存中动态生成的代理对象
        System.out.println(proxy.getClass());

        // 执行方法   【代理对象】
        proxy.save();
    }
}
```
 

结果:
```
class ReflectTest.UserDao
class com.sun.proxy.$Proxy0
开始事务2
----已经保存数据!----
提交事务2
```

#### 3.详解Proxy类

