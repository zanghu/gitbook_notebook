## 初学


### 1.概念

1.1. 登录本地MySQL服务

```shell
$ mysql -u zanghu -p              # 成功
$ mysql -u zanghu -P -h 127.0.0.1 # 成功
$ mysql -u zanghu -P -h localhost # 失败，据说原因是mysql连接localhost默认使用unix域sokcet，
                                # 但是具体为什么会失败原因没查到
```

1.2. 登陆后查看当前用户权限

```mysql
mysql> show grants;             # 成功
mysql> show grants for [用户名]; # 失败
```

1.3. 查看当前连接的数据库

```mysql
mysql> select database();
```

1.4. 连接指定数据库

```mysql
mysql> use [数据库名称];
```

1.5. 查看数据库中所有表名

```mysql
mysql> show tables;                # 成功
mysql> show tables from [数据库名]; # 成功
```

1.6. 查看表中所有字段名

```mysql
# 注意下面的命令中 president 和 sampdb 都用反引号'`'括起来了
# 使用反引号一般是为了避免 MySQL 关键字域字段名、表名、数据库名称冲突，目前成为惯例
mysql> show columns from `president` from `sampdb`;
```

### 2.脚本

通过数据库脚本（*.sql）建表

脚本内容（注意表明、字段名被反引号括起来了）：

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

执行脚本的命令：

```shell
$ mysql -u [用户名] -p -D [数据库名] < [建库脚本路径]
$ mysql -u zanghu -p -D sampdb < /home/zanghu/code_box/mysql_code/sampdb/create_president.sql
```
