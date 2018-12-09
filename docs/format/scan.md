# `sf::scan`
Defined in [`<sf/format.hpp>`](./index.md).
``` c++
// 1
template <
    typename... Args
> constexpr std::istream& scan(sf::string_view fmt, Args&&... args);

// 2
template <
    typename... Args
> constexpr std::istream& scan(std::istream& stream, sf::string_view fmt, Args&&... args);

// 3
template <
    typename... Args
> constexpr std::wistream& scan(sf::wstring_view fmt, Args&&... args);

// 4
template <
    typename... Args
> constexpr std::wistream& scan(std::wistream& stream, sf::wstring_view fmt, Args&&... args);

// 5
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename... Args
> constexpr std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, sf::basic_string_view<Char, Traits> fmt, Args&&... args);

// 6
#ifndef SF_FORCE_WIDE_IO
template <
    typename T
> constexpr std::istream& scan(T&& arg);
#else
template <
    typename T
> constexpr std::wistream& scan(T&& arg);
#endif // !SF_FORCE_WIDE_IO

// 7
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename T
> constexpr std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, T&& arg);
```

`scan` is much like [`print`](./print.md). *1* and *6* (if `SF_FORCE_WIDE_IO` not defined) scans from `std::cin`, *3* and *6* (if `SF_FORCE_WIDE_IO` defined) scans from `std::wcin`.

Pass an argument with type `T&&` is well-defined, if and only if `T` is *MoveConstructible*.
