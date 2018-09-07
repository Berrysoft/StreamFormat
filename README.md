# StreamFormat
A C++ format IO library, based on standard streams.
## Print
The usage of `print` function is like .NET `Console.Write`:
``` c++
print("Hello, {0}!\n", "world");            //Hello, world!
print("The meaning of life: {0}.\n", 42);   //The meaning of life: 42.
print("{0}{1}{0}", "abra", "cad");          //abracadabra
```
And you can `print` to other streams:
``` c++
print(std::clog, "{0}:{1}", 11, 59);
std::ostringstream oss;
print(oss, "This message is printed\n") << "And this is shifted." << std::endl;
```
`print` returns the stream object you have just printed.
## Scan
`scan` function is a bit like `print`, and actually they share most of the codes:
``` c++
//Input: 123 456
int a, b;
scan("{0}{1}", a, b);
//The following one is OK:
scan("{0} {1}", a, b);
// Now output:
print("a = {0}, b = {1}\n", a, b);//a = 123, b = 456

std::istringstream iss("HTTP/1.1");
double ver;
scan(iss, "HTTP/{0}", ver);
print("The HTTP version: {0}", ver);
```
## Declaration
``` c++
namespace stream_format
{
    //char IO
    template <typename... Args>
    constexpr std::istream& scan(std::istream& is, std::string_view fmt, Args&... args);
    template <typename... Args>
    constexpr std::istream& scan(std::string_view fmt, Args&... args);
    template <typename... Args>
    constexpr std::ostream& print(std::ostream& os, std::string_view fmt, Args&&... args);
    template <typename... Args>
    constexpr std::ostream& print(std::string_view fmt, Args&&... args);

    //wchar_t IO
    template <typename... Args>
    constexpr std::wistream& scan(std::wistream& is, std::wstring_view fmt, Args&... args);
    template <typename... Args>
    constexpr std::wistream& scan(std::wstring_view fmt, Args&... args);
    template <typename... Args>
    constexpr std::wostream& print(std::wostream& os, std::wstring_view fmt, Args&&... args);
    template <typename... Args>
    constexpr std::wostream& print(std::wstring_view fmt, Args&&... args);

    //template IO
    template <typename Char, typename... Args, typename Traits = std::char_traits<Char>>
    constexpr std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&... args);
    template <typename Char, typename... Args, typename Traits = std::char_traits<Char>>
    constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args)
}
```
