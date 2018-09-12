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
And you can `print` to other streams:
``` c++
using namespace std;
sf::print(std::clog, "{0}:{1}", 11, 59);
ostringstream oss;
sf::print(oss, "This message is printed.\n") << "And this is shifted." << endl;
```
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
sf::scan(iss, "HTTP/{0}", ver);
sf::print("The HTTP version: {0:f1}", ver);//The HTTP version: 1.1
```
`sscan` returns a part of the string which wasn't scanned:
``` c++
using namespace std;
int i; string s;
string ret = sf::sscan("123 abc !!!", "{0}{1}", i, s);//ret == " !!!"
```
### Colors
`make_color_arg` function helps you to wrap an argument with specified foreground and background color:
``` c++
sf::print("{0}, {1}!\n", sf::make_color_arg("Hello", yellow), sf::make_color_arg("world", cyan, true, blue, false));
```
