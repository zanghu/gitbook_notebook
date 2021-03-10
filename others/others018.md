## git bash 使用本地代理

**参考资料**


### 1.git设置代理相关命令

```shell
# 设置代理
git config --global http.proxy 代理ip:代理端口
git config --global https.proxy 代理ip:代理端口

# 清除代理设置
git config --global --unset http.proxy
git config --global --unset https.proxy

# 查看当前代理设置
git config --global --get http.proxy
git config --global --get https.proxy

```

