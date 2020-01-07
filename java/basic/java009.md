### Spring: 依赖注入和循环依赖

参考资料：[cnblogs: 依赖注入原理---IoC框架](https://www.cnblogs.com/sunzhao/p/8334008.html)


参考资料：[CSDN: 什么是循环依赖以及解决方式](https://blog.csdn.net/bingguang1993/article/details/88915576)

#### 1.依赖注入

先来讲一讲，一个简单的依赖注入例子。

依赖 如果在 Class A 中，有 Class B 的实例，则称 Class A 对 Class B 有一个依赖。例如下面类 Human 中用到一个 Father 对象，我们就说类 Human 对类 Father 有一个依赖。

```java
public class Human {
    ...
    Father father;
    ...
    public Human() {
        father = new Father();
    }
}
```

仔细看这段代码我们会发现存在一些问题：
(1). 如果现在要改变 father 生成方式，如需要用new Father(String name)初始化 father，需要修改 Human 代码；
(2). 如果想测试不同 Father 对象对 Human 的影响很困难，因为 father 的初始化被写死在了 Human 的构造函数中；
(3). 如果new Father()过程非常缓慢，单测时我们希望用已经初始化好的 father 对象 Mock 掉这个过程也很困难。

依赖注入 上面将依赖在构造函数中直接初始化是一种 Hard init 方式，弊端在于两个类不够独立，不方便测试。我们还有另外一种 Init 方式，如下：

```java
public class Human {
    ...
    Father father;
    ...
    public Human(Father father) {
        this.father = father;
    }
}
```

上面代码中，我们将 father 对象作为构造函数的一个参数传入。在调用 Human 的构造方法之前外部就已经初始化好了 Father 对象。像这种非自己主动初始化依赖，而通过外部来传入依赖的方式，我们就称为依赖注入。
现在我们发现上面 1 中存在的两个问题都很好解决了，简单的说依赖注入主要有两个好处：
(1). 解耦，将依赖之间解耦。
(2). 因为已经解耦，所以方便做单元测试，尤其是 Mock 测试。

#### 2.循环依赖

* 什么是循环依赖？

  它发生在bean A依赖于另一个bean B时，bean B依赖于bean A：

  豆A→豆B→豆A

  当然，我们可以有更多的暗示：

  豆A→豆B→豆C→豆D→豆E→豆A

* spring会发生什么

  当Spring上下文加载所有bean时，它会尝试按照它们完全工作所需的顺序创建bean。例如，如果我们没有循环依赖，如下例所示：

  豆A→豆B→豆C.

  Spring将创建bean C，然后创建bean B（并将bean注入其中），然后创建bean A（并将bean B注入其中）。

  但是，当具有循环依赖时，Spring无法决定应该首先创建哪个bean，因为它们彼此依赖。在这些情况下，Spring将在加载上下文时引发BeanCurrentlyInCreationException。

  使用构造函数注入时，它可能发生在Spring中; 如果您使用其他类型的注入，则不应该发现此问题，因为依赖项将在需要时注入，而不是在上下文加载时注入。

* 一个快速示例

  让我们定义两个相互依赖的bean（通过构造函数注入）：

```java
@Component
public class CircularDependencyA {

    private CircularDependencyB circB;

    @Autowired
    public CircularDependencyA(CircularDependencyB circB) {
        this.circB = circB;
    }
}
```

```java
@Component
public class CircularDependencyB {

    private CircularDependencyA circA;

    @Autowired
    public CircularDependencyB(CircularDependencyA circA) {
        this.circA = circA;
    }
}
```
