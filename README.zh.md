# StreamFormat
一个以标准输入输出流为基础的C++格式化输入输出库。
## 使用方法
### 输出
`print`函数用起来像.NET中的`Console.Write`：
``` c++
sf::print("Hello, {0}!\n", "world");            //Hello, world!
sf::print("生命、宇宙以及一切：{0}\n", 42);   //生命、宇宙以及一切：42
sf::print("0x{0:x8}\n", 4276215469);            //0xfee1dead
```
也可以写起来像Python：
``` c++
sf::print("The big {} {} ", "brown", "fox");
//如果不写序号，它会自动递增：
sf::print("{2} over the {0} {}.\n", "lazy", "dog", "jumps");
```
如果序号大于等于参数个数，这一部分不会被格式化：
``` c++
sf::print("{0}{10000}{0}\n", "123");//123{10000}123
```
你也可以`print`到其他流：
``` c++
sf::print(std::clog, "{0}:{1}", 11, 59);
std::ostringstream oss;
sf::print(oss, "通过print输出\n") << "通过移位运算符输出" << endl;
```
`println`是一个类似的函数，就是在`print`后面加上了`std::endl`。由于`std::endl`会调用`flush`，`println("...")`和`print("...\n")`并不完全一样。

`print`的返回值是它操作的流，`sprint`则可以返回一个字符串：
``` c++
std::string s = sf::sprint("{0}{{{1}}}{0}", "123", "321");//123{321}123
```
### 输入
`scan`和`print`很像，实际上它们共用了大部分代码：
``` c++
int a, b;
//输入：123 456
sf::scan("{0}{1}", a, b);
//这么做也可以：
sf::scan("{0} {1}", a, b);
//输出：
sf::print("a = {0}, b = {1}\n", a, b);//a = 123, b = 456

std::istringstream iss("HTTP/1.1");
double ver;
sf::scan(iss, "HTTP/{}", ver);
sf::print("HTTP版本：{0:f1}", ver);//HTTP版本：1.1
```
把一个右值引用传给`scan`是良好定义的，当且仅当这个参数类型是*可移动构造*的。

`sscan`返回未被输入部分的字符串在原字符串的起始序号：
``` c++
std::string str = "123 abc !!!";
int i; std::string s;
std::string ret = str.substr(sf::sscan(str, "{0}{1}", i, s));//ret == " !!!"
```
### 颜色
`make_color_arg`函数打包一个需要格式化的参数，及其前景色、背景色与字体样式。
``` c++
sf::print("{0}, {1}!\n", sf::make_color_arg("Hello", yellow), sf::make_color_arg("world", bright_cyan, blue, underline));
```
