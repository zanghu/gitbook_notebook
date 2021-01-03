## 初学


### 1.概念

登录本地数据库

```shell
mysql -u zanghu -p              # 成功
mysql -u zanghu -P -h 127.0.0.1 # 成功
mysql -u zanghu -P -h localhost # 失败，据说原因是mysql连接localhost默认使用unix域sokcet，
                                # 但是具体为什么会失败原因没查到
```

登陆后查看当前用户权限

mysql> show grants
```