# `sf::sprint`
Defined in [`<sf/sformat.hpp>`](./index.md).
``` c++
// 1
template <
    typename... Args
> constexpr std::string sprint(sf::string_view fmt, Args&&... args);

// 2
template <
    typename... Args
> constexpr std::wstring sprint(sf::wstring_view fmt, Args&&... args);

// 3
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename Allocator = std::allocator<Char>, 
    typename... Args
> constexpr std::basic_string<Char, Traits, Allocator> sprint(sf::basic_string_view<Char, Traits> fmt, Args&&... args);
```

|Param|Summary|
|-|-|
|`fmt`|The format string.|
|`args...`|Variable-length arguments.|

`sprint` is much like [`print`](../format/print.md).
