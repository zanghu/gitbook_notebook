## 初学


### 1.基本常用操作

1.1. 登录本地MySQL服务

```shell
$ mysql -u zanghu -p              # 成功
$ mysql -u zanghu -P -h 127.0.0.1 # 成功
$ mysql -u zanghu -P -h localhost # 失败，据说原因是mysql连接localhost默认使用unix域sokcet，
                                # 但是具体为什么会失败原因没查到
```

1.2. 登陆后查看当前用户权限

```sql
mysql> show grants;             # 成功
mysql> show grants for [用户名]; # 失败
```

1.3. 查看当前连接的数据库

```sql
mysql> select database();
```

1.4. 连接指定数据库

```sql
mysql> use [数据库名称];
```

1.5. 查看数据库中所有表名

```sql
mysql> show tables;                # 成功
mysql> show tables from [数据库名]; # 成功
mysql> show tables in [数据库名];   # 成功
```

1.6. 查看表中所有字段名

```sql
# 注意下面的命令中 president 和 sampdb 都用反引号'`'括起来了
# 使用反引号一般是为了避免 MySQL 关键字域字段名、表名、数据库名称冲突，目前成为惯例

# 方法一：show 关键字
mysql> show columns from `president` from `sampdb`; # 成功

# 方法二：describe 关键字
mysql> describe [表名];           # 成功
mysql> describe [数据库名].[表名]; # 成功
```

### 2.执行SQL脚本

2.1. 执行shell命令，通过数据库脚本（*.sql）建表

脚本内容（注意其中的表名、字段名都被反引号括起来了）：

```sql
DROP TABLE IF EXISTS `president`;
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

执行脚本的命令：

```shell
$ mysql -u [用户名] -p -D [数据库名] < [建库脚本路径]
$ mysql -u zanghu -p -D sampdb < /home/zanghu/code_box/mysql_code/sampdb/create_president.sql
```

2.2. 在mysql命令行内，执行建表脚本建表

建表脚本内容：

```sql
DROP TABLE IF EXISTS `member`;
CREATE TABLE `member`
(
    `member_id`  INT UNSIGNED NOT NULL AUTO_INCREMENT,
    PRIMARY KEY (`member_id`),
    `last_name`  VARCHAR(20) NOT NULL,
    `first_name` VARCHAR(20) NOT NULL,
    `suffix`     VARCHAR(5)  NULL,
    `expiration` DATE NULL,
    `email`      VARCHAR(100)  NULL,
    `street`     VARCHAR(50)  NULL,
    `city`       VARCHAR(50) NOT NULL,
    `state`      VARCHAR(2)  NOT NULL,
    `zip`        VARCHAR(10) NULL,
    `phone`      VARCHAR(20) NULL,
    `interests`  VARCHAR(255) NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

```mysql
# 使用 source 关键字
mysql> source /home/zanghu/code_box/mysql_code/sampdb/create_member.sql;
```
