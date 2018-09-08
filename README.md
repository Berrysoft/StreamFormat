# StreamFormat
A C++ format IO library, based on standard streams.
## Print
The usage of `print` function is like .NET `Console.Write`:
``` c++
print("Hello, {0}!\n", "world");            //Hello, world!
print("Meaning of the life: {0}.\n", 42);   //Meaning of the life: 42.
print("0x{0:x8}\n", 4276215469);            //0xfee1dead
```
And you can `print` to other streams:
``` c++
print(std::clog, "{0}:{1}", 11, 59);
std::ostringstream oss;
print(oss, "This message is printed.\n"sv) << "And this is shifted." << std::endl;
```
To make compilers happy, you may add an `s` or `sv` after some string literals.

`print` returns the stream object you have just printed, while `sprint` returns a string:
``` c++
std::string s = sprint("{0}{{{1}}}{0}", "abra", "cad");//abra{cad}abra
```
## Scan
`scan` function is a bit like `print`, and actually they share most of the codes:
``` c++
int a, b;
//Input: 123 456
scan("{0}{1}", a, b);
//The following one is OK:
scan("{0} {1}", a, b);
// Now output:
print("a = {0}, b = {1}\n", a, b);//a = 123, b = 456

std::istringstream iss("HTTP/1.1");
double ver;
scan(iss, "HTTP/{0}", ver);
print("The HTTP version: {0:f1}", ver);//The HTTP version: 1.1
```
`sscan` returns a part of the string which wasn't scanned:
``` c++
int i; std::string s;
std::string ret = sscan("123 abc !!!"s, "{0}{1}"sv, i, s);//ret == " !!!"
```
## Declaration
`format.hpp`
``` c++
namespace sf
{
    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    constexpr std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&... args);
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args);

    //char IO
    template <typename... Args>
    constexpr std::istream& scan(std::string_view fmt, Args&... args);
    template <typename... Args>
    constexpr std::ostream& print(std::string_view fmt, Args&&... args);

    //wchar_t IO
    template <typename... Args>
    constexpr std::wistream& scan(std::wstring_view fmt, Args&... args);
    template <typename... Args>
    constexpr std::wostream& print(std::wstring_view fmt, Args&&... args);
} // namespace sf
```
`sformat.hpp`
``` c++
namespace sf
{
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    constexpr std::basic_string<Char, Traits, Alloc> sscan(const std::basic_string<Char, Traits, Alloc>& str, std::basic_string_view<Char, Traits> fmt, Args&... args);
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    constexpr std::basic_string<Char, Traits, Alloc> sprint(std::basic_string_view<Char, Traits> fmt, Args&&... args);
} // namespace sf
```
