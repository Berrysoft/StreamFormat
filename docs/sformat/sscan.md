# `sf::sscan`
Defined in [`<sf/sformat.hpp>`](./index.md)
``` c++
// 1
template <
    typename... Args
> constexpr std::streampos sscan(const std::string& str, std::string_view fmt, Args&&... args);

// 2
template <
    typename... Args
> constexpr std::wstreampos sscan(const std::wstring& str, std::wstring_view fmt, Args&&... args);

// 3
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename Allocator = std::allocator<Char>, 
    typename... Args
> constexpr typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, std::basic_string_view<Char, Traits> fmt, Args&&... args);
```

|Param|Summary|
|-|-|
|`str`|The input string.|
|`fmt`|The format string.|
|`args...`|Variable-length arguments.|

`sscan` is much like [`scan`](../format/scan.md), but it returns a position from which the remain string starts.
