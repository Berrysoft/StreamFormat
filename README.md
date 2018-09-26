# StreamFormat
A C++ format IO library, based on standard streams.
## Usage
### Print
The usage of `print` function is like .NET `Console.Write`:
``` c++
sf::print("Hello, {0}!\n", "world");            //Hello, world!
sf::print("Meaning of the life: {0}.\n", 42);   //Meaning of the life: 42.
sf::print("0x{0:x8}\n", 4276215469);            //0xfee1dead
```
And you can write it like Python:
``` c++
sf::print("The big {} {} ", "brown", "fox");
//If you don't write the index, it will increase automatically.
sf::print("{2} over the {0} {}.\n", "lazy", "dog", "jumps");
```
If the index is larger than the size of the arguments, it won't be formatted:
``` c++
sf::print("{0}{10000}{0}\n", "123");//123{10000}123
```
Additionally, you can `print` to other streams:
``` c++
sf::print(std::clog, "{0}:{1}", 11, 59);
std::ostringstream oss;
sf::print(oss, "This message is printed.\n") << "And this is shifted." << endl;
```
`println` is a similar function, which will `<< std::endl` after `print`. As `std::endl` will `flush` the stream, `println("...")` is a little different from `print("...\n")`.

`print` returns the stream object you have just printed, while `sprint` returns a string:
``` c++
std::string s = sf::sprint("{0}{{{1}}}{0}", "123", "321");//123{321}123
```
### Scan
`scan` function is a bit like `print`, and actually they share most of the codes:
``` c++
int a, b;
//Input: 123 456
sf::scan("{0}{1}", a, b);
//The following one is OK:
sf::scan("{0} {1}", a, b);
// Now output:
sf::print("a = {0}, b = {1}\n", a, b);//a = 123, b = 456

std::istringstream iss("HTTP/1.1");
double ver;
sf::scan(iss, "HTTP/{}", ver);
sf::print("The HTTP version: {0:f1}", ver);//The HTTP version: 1.1
```
It is an well-defined behavior to pass an rvalue to `scan`, if and only if the type of the argument is *MoveConstructible*.

`sscan` returns an index from which the part of the string that wasn't scanned starts:
``` c++
std::string str = "123 abc !!!";
int i; std::string s;
std::string ret = str.substr(sf::sscan(str, "{0}{1}", i, s));//ret == " !!!"
```
### Colors
`make_color_arg` function helps you to wrap an argument with specified foreground color, background color and font style:
``` c++
sf::print("{0}, {1}!\n", sf::make_color_arg("Hello", yellow), sf::make_color_arg("world", bright_cyan, blue, underline));
```
