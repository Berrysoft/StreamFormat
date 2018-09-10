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
sf::print(oss, "This message is printed.\n"sv) << "And this is shifted." << endl;
```
To make compilers happy, you may add an `s` or `sv` after some string literals.

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
string ret = sf::sscan("123 abc !!!"s, "{0}{1}"sv, i, s);//ret == " !!!"
```
### Colors
`make_color_arg` function helps you to wrap an argument with specified foreground and background color:
``` c++
sf::print("{0}, {1}!\n", sf::make_color_arg("Hello", yellow), sf::make_color_arg("world", cyan, true, blue, false));
```
## Declaration
format.hpp
``` c++
namespace sf
{
    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    constexpr std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args);
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args);

    //char IO
    template <typename... Args>
    constexpr std::istream& scan(std::string_view fmt, Args&&... args);
    template <typename... Args>
    constexpr std::ostream& print(std::string_view fmt, Args&&... args);

    //wchar_t IO
    template <typename... Args>
    constexpr std::wistream& scan(std::wstring_view fmt, Args&&... args);
    template <typename... Args>
    constexpr std::wostream& print(std::wstring_view fmt, Args&&... args);
} // namespace sf
```
sformat.hpp
``` c++
namespace sf
{
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    constexpr std::basic_string<Char, Traits, Alloc> sscan(const std::basic_string<Char, Traits, Alloc>& str, std::basic_string_view<Char, Traits> fmt, Args&&... args);
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    constexpr std::basic_string<Char, Traits, Alloc> sprint(std::basic_string_view<Char, Traits> fmt, Args&&... args);
} // namespace sf
```
ansi.hpp
``` c++
namespace sf
{
    template <typename... Args>
    class ansi_control
    {
    public:
        ansi_control(Args&&... args);
        template <typename Char, typename Traits = std::char_traits<Char>>
        friend constexpr std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const ansi_control& ctrl);
    };

    enum color : int
    {
        black = 0,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white,
        extendend,
        user_defaults
    };

    template <typename T>
    class color_arg
    {
    public:
        constexpr color_arg();
        constexpr color_arg(T&& arg, color fore, bool foreb, color back, bool backb);
        template <typename Char, typename Traits = std::char_traits<Char>>
        friend constexpr std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color_arg<T>& arg);
    };

    template <typename... Args>
    constexpr ansi_control<Args...> make_ansi_control(Args&&... args);

    template <typename T>
    constexpr color_arg<T> make_color_arg(T&& arg, color fore, bool foreb = false);
    template <typename T>
    constexpr color_arg<T> make_color_arg(T&& arg, color fore, color back);
    template <typename T>
    constexpr color_arg<T> make_color_arg(T&& arg, color fore, bool foreb, color back, bool backb);
} // namespace sf
```
