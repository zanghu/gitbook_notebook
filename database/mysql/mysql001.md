## 初学


### 1.概念

登录本地MySQL服务

```shell
$ mysql -u zanghu -p              # 成功
$ mysql -u zanghu -P -h 127.0.0.1 # 成功
$ mysql -u zanghu -P -h localhost # 失败，据说原因是mysql连接localhost默认使用unix域sokcet，
                                # 但是具体为什么会失败原因没查到
```

登陆后查看当前用户权限

```mysql
mysql> show grants;             # 成功
mysql> show grants for [用户名]; # 失败
```

查看当前连接的数据库

```mysql
mysql> select database();
```

连接指定数据库

```mysql
mysql> use [数据库名称];
```

查看数据库中所有表名

```mysql
mysql> show tables;                # 成功
mysql> show tables from [数据库名]; # 成功
```

### 2.脚本

通过数据库脚本（*.sql）建表

```shell
$ mysql -u [用户名] -p -D [数据库名] < [建库脚本路径]
$ mysql -u zanghu -p -D sampdb < /home/zanghu/code_box/mysql_code/sampdb/create_president.sql
```

