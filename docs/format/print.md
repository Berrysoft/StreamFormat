# `sf::print`
Defined in [`<sf/format.hpp>`](./index.md)
``` c++
// 1
template <
    typename... Args
> constexpr std::ostream& print(std::string_view fmt, Args&&... args);

// 2
template <
    typename... Args
> constexpr std::ostream& print(std::ostream& stream, std::string_view fmt, Args&&... args);

// 3
template <
    typename... Args
> constexpr std::wostream& print(std::wstring_view fmt, Args&&... args);

// 4
template <
    typename... Args
> constexpr std::wostream& print(std::wostream& stream, std::wstring_view fmt, Args&&... args);

// 5
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename... Args
> constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args);
```

|Param|Summary|
|-|-|
|`stream`|The stream to output.|
|`fmt`|The format string.|
|`args...`|Variable-length arguments.|

*1* prints to `std::cout` and *3* prints to `std::wcout`.

The `fmt` string refers an argument by its index, starts with 0, and embrace the index with `{}`. If the index embraced is out of range, it won't be formatted.

You can specify the format style of the argument, with the syntax `{<index>:<flag>[<number>][,<flag>[<number>]...]}`. The flag range from:

|Flag|Summary|
|-|-|
|b|boolalpha|
|d|dec, fix to length `number` with '0'|
|e|scientific|
|f|fixed|
|g|general(nop)|
|i|internal|
|l|left, fix to length `number` with space|
|o|oct, fix to length `number` with '0'|
|r|right, fix to length `number` with space|
|u|uppercase|
|x|hex, fix to length `number` with '0'|

``` c++
// 6
#ifndef SF_FORCE_WIDE_IO
template <
    typename T
> constexpr std::ostream& print(T&& arg);
#else
template <
    typename T
> constexpr std::wostream& print(T&& arg);
#endif // !SF_FORCE_WIDE_IO

// 7
template <
    typename Char, 
    typename Traits = std::char_traits<Char>, 
    typename T
> constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, T&& arg);
```

|Param|Summary|
|-|-|
|`stream`|The stream to output.|
|`arg`|Argument to print.|

*6* prints to `std::cout` (if `SF_FORCE_WIDE_IO` not defined) or `std::wcout` (if `SF_FORCE_WIDE_IO` defined).

A simple implementation of *7* is:
``` c++
template <typename Char, typename Traits = std::char_traits<Char>, typename T>
constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, T&& arg)
{
    return stream << std::forward<T>(arg);
}
```

All `print` functions return the `stream` printed to.
