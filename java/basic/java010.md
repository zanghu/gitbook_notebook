### service、domain、entity和DAO的使用场景

参考资料：[cnblogs: vo类，model类，dto类的作用及划分](https://www.cnblogs.com/liuhaobin/p/10709713.html)

1、entity里的每一个字段，与数据库相对应，

2、dto里的每一个字段，是和你前台页面相对应，

3、VO，这是用来转换从entity到dto，或者从dto到entity的中间的东西。

 
举个例子：

你的html页面上有三个字段，name，pass，age

你的数据库表里，有两个字段，name，pass(注意没有age哦)而你的dto里，就应该有下面三个(因为对应html页面上三个字段嘛)

```java
private string name；
private string pass; 
private string age;
```

这个时候，你的entity里，就应该有两个(因为对应数据库表中的2个字段嘛)

```java
private string name；
private string pass;
```

到了这里，好了，业务经理让你做这样一个业务“年龄大于20的才能存入数据库”

这个时候，你就要用到vo了

你要先从页面上拿到dto，然后判断dto中的age是不是大于20，如果大于20，就把dto中的

name和pass拿出来，放到vo中，然后在把vo中的name和pass原封不懂的给entity，然后根据

entity的值，在传入数据库，这就是他们三个的区别

PS，VO和entity里面的字段应该是一样的，vo只是entity到dto，或者dto到entity的中间过程，如果没有这个过程，你仍然可以做到增删改查，这个就根据具体公司来的