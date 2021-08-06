## python实现使用SMTP协议发邮件

参考资料

[SMTP发送邮件](https://www.liaoxuefeng.com/wiki/897692888725344/92305714496428)

### 例子

```python
# -*- coding: utf-8 -*-
from email import encoders
from email.header import Header
from email.mime.text import MIMEText
from email.utils import parseaddr, formataddr
import smtplib

def _format_addr(s):
    """"""
    name, addr = parseaddr(s)
    return formataddr(( \
        Header(name, 'utf-8').encode(), \
        addr.encode('utf-8') if isinstance(addr, unicode) else addr))

def main():
    """"""
    from_addr = r"xxx@126.com" # 发送方邮箱
    password = r"xxx" # 发送方密码
    to_addr = r"yyy@sina.com" # 接收方邮箱
    smtp_server = r"smtp.126.com" # SMTP邮件服务器地址

    msg = MIMEText('hello, send by Python...', 'plain', 'utf-8')
    #msg['From'] = _format_addr(u'Python爱好者 <%s>' % from_addr)
    #msg['To'] = _format_addr(u'管理员 <%s>' % to_addr)
    #msg['Subject'] = Header(u'来自SMTP的问候……', 'utf-8').encode()

    server = smtplib.SMTP(smtp_server, 25)
    server.set_debuglevel(1)
    server.login(from_addr, password)
    server.sendmail(from_addr, [to_addr], msg.as_string())
    server.quit()
```

