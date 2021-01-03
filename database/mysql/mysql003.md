## MySQL: IF NOT EXISTS和 IF EXISTS

反引号，就是键盘上`tab`键上面的那个按键

### 1.用法

`if not exists`：即如果不存在，一般在建表、建库、增加字段时使用

`if exists`：即如果存在，一般在删表、删库、删字段时使用

### 2.示例

建表

```sql
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








