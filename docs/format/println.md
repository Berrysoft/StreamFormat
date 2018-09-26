# `sf::println`
Defined in [`<sf/format.hpp>`](./index.md)
``` c++
// 1
template <
    typename... Args
> constexpr std::ostream& println(std::string_view fmt, Args&&... args);

// 2
template <
    typename... Args
> constexpr std::ostream& println(std::ostream& stream, std::string_view fmt, Args&&... args);

// 3
template <
    typename... Args
> constexpr std::wostream& println(std::wstring_view fmt, Args&&... args);

// 4
template <
    typename... Args
> constexpr std::wostream& println(std::wostream& stream, std::wstring_view fmt, Args&&... args);

// 5
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename... Args
> constexpr std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args);

// 6
#ifndef SF_FORCE_WIDE_IO
template <
    typename T
> constexpr std::ostream& println();
#else
template <
    typename T
> constexpr std::wostream& println();
#endif // !SF_FORCE_WIDE_IO

// 7
template <
    typename Char, 
    typename Traits = std::char_traits<Char>
> constexpr std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream);

// 8
#ifndef SF_FORCE_WIDE_IO
template <
    typename T
> constexpr std::ostream& println(T&& arg);
#else
template <
    typename T
> constexpr std::wostream& println(T&& arg);
#endif // !SF_FORCE_WIDE_IO

// 9
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename T
> constexpr std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream, T&& arg);
```

`println` is much like [`print`](./print.md). *1* and *6*, *8* (if `SF_FORCE_WIDE_IO` not defined) prints to `std::cout`, *3* and *6*, *8* (if `SF_FORCE_WIDE_IO` defined) prints to `std::wcout`.

Actually, the only difference between `println` and `print` is that `println(...)` <=> `print(...) << std::endl`.

*6* and *7* are two special functions, they only print a new line and flushes the stream. Here is a simple implemetation:
``` c++
template <typename Char, typename Traits = std::char_traits<Char>>
constexpr std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream)
{
    return stream << std::endl;
}
```
