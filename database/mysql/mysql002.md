## MySQL: 单引号、双引号和反引号的用法区别

先我们先来说一下单引号和反引号，如图，这里的   '图书ID'   就使用到了单引号，而

 CREATE TABLE `book`  这里的  book  就使用到了反引号，那么他们具体的有什么区别呢？

数据库中引号的用法（mysql、oracle、plsql）
单引号：我们在例子中的条件值周围使用的是单引号。SQL 使用单引号来环绕文本值。如果是数值，请不要使用引号。

按照别的说法来说就是Varchar类型（也可以说是String、字符串类型）这一些在数据库语句中使用的时候应该使用单引号，而不是直接使用。

而对于数值类型的，反而是不能使用单引号。

如图所示有正确的和错误的使用方法：

数据库中引号的用法（mysql、oracle、plsql）
数据库中引号的用法（mysql、oracle、plsql）

### 2.反引号

反引号：它是为了区分MYSQL的保留字与普通字符而引入的符号。

所谓的保留字就是select database insert 这一类数据库的sql指令，当我们不得已要拿他们来做表名和字段名的时候 我们必须要加反引号来避免编译器把这部分认为是保留字而产生错误。

例如：如下的建库脚本（注意表明、字段名被反引号括起来了）：

```mysql
CREATE TABLE `president`
(
    `last_name`  VARCHAR(15) NOT NULL,
    `first_name` VARCHAR(15) NOT NULL,
    `suffix`     VARCHAR(5)  NULL,
    `city`       VARCHAR(20) NOT NULL,
    `state`      VARCHAR(2)  NOT NULL,
    `birth`      DATE        NOT NULL,
    `death`      DATE        NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

务必要记住：保留字既不能作为表名，也不能作为字段名，如果非要这么操作，请记住要增加反引号！

4
双引号的用法和单引号有所类似，大多数数据库都支持单引号和双引号的互换，即varchar类型的变量既可以用单引号来囊括，也可以用双引号。

当然了，一边单引号，一边双引号是不被允许的。
```

### 2.脚本

通过数据库脚本（*.sql）建表




