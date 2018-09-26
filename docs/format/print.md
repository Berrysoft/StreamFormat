# `sf::print`
Defined in [`<sf/format.hpp>`](./index.md)
``` c++
template <
    typename... Args
> constexpr std::ostream& print(std::string_view fmt, Args&&... args);

template <
    typename... Args
> constexpr std::wostream& print(std::wstring_view fmt, Args&&... args);

template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename... Args
> constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args);
```
