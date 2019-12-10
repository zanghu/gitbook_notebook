## HTML的script标签

参考资料：[W3school: HTML &ltscript&gt 标签](https://www.w3school.com.cn/tags/tag_script.asp)

参考资料：[W3school: HTML 5 &ltscript&gt 标签](https://www.w3school.com.cn/html5/tag_script.asp)

参考资料：[cnblogs: 关于在HTML中使用的script标签](https://www.cnblogs.com/wangyue99599/p/7275527.html)

HTML4和HTML5标准中对script标签属性的必填项、默认值的要求有较大区别

### 1. 区别

HTML 4.01 与 HTML 5 之间的差异
在 HTML 4 中，"type" 属性是必需的，但在 HTML5 中是可选的。

"async" 是 HTML5 中的新属性。

HTML5 中不再支持 HTML 4.01 中的某些属性。

HTML4中`script`标签的唯一必填属性`type`在HTML5中不再是必填项，默认值为`"text/javascript"`

HTML5中的属性

| 属性明 | 描述 | 是否必填 |
| :---: | --- | --- |
| type | 指示脚本的 MIME 类型。 | 否 |
| async | 规定异步执行脚本（仅适用于外部脚本）。| 否 |
| charset | 规定在外部脚本文件中使用的字符编码。大多数浏览器已经忽略它的值了，所以很少有人使用。| 否 |
| defer	| 规定是否对脚本执行进行延迟，直到页面加载为止。| 否 |
| language | 不赞成使用。规定脚本语言。请使用 type 属性代替它。| 否 |
| src | 规定外部脚本文件的 URL。注意：在引用外部文件，标签中不要加入其它JS代码，浏览器在解析时，只会下载src引用的外部脚本文件，表中内嵌入的代码将会被忽略。| 否 |
| xml:space | 规定是否保留代码中的空白。| 否 |


### 2.script标签的位置

通常，我们会把带有外部文件的的标签（包括CSS文件，JavaScript文件）的引用放在相同的位置，一般是在&lthead&gt标签里面。

　　但是在解析过程中一旦遇到多个JavaScript外部文件，就要等到所有的外部文件加载完成后，页面才能完全显示，所以通常我们会把它放在&ltbody&gt标签里的底部，如下所示：

![](/assets/front003_01.png)

　　在上面中提到过&ltscript&gt中有defer这个属性，但是由于在HTML5中提到过，HTML5会忽略嵌入脚本所设置defer属性，目前只有IE4~IE7还支持defer属性，IE8以后完全遵循HTML5的标准，所以把&ltscript&gt放在&ltbody&gt标签里的底部依旧是最佳选择。

### 3.noscript标签的位置

字面意思，NO-script，没有script，也就是浏览器不支持JavaScript时，<noscript>标签中内容才会被显示，

* 浏览器不支持脚本；
* 浏览器支持脚本，但是JavaScript被禁用；
　
以上两条符合任何一个，&ltnoscript&gt标签内的内容都会被显示。

