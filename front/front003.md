## HTML的script标签

[W3school: HTML &ltscript&gt 标签](https://www.w3school.com.cn/tags/tag_script.asp)

[W3school: HTML 5 &ltscript&gt 标签](https://www.w3school.com.cn/html5/tag_script.asp)

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
| charset | 规定在外部脚本文件中使用的字符编码。| 否 |
| defer	| 规定是否对脚本执行进行延迟，直到页面加载为止。| 否 |
| language | 不赞成使用。规定脚本语言。请使用 type 属性代替它。| 否 |
| src | 规定外部脚本文件的 URL。| 否 |
| xml:space | 规定是否保留代码中的空白。| 否 |


### 2.script标签的位置

