# `sf::make_ansi_control`
Defined in [`<sf/index.hpp>`](./index.md)
``` c++
template <
    typename... Args
> constexpr /*unspecified*/ make_ansi_control(Args&&... args);
```

|Param|Summary|
|-|-|
|args...|Variable-length arguments.|

The return type of `make_ansi_control` must be *Outputable*, that is, `stream << make_ansi_control(...)` must be valid if `stream` is `std::basic_ostream`. When output, it will print `"\033[<arg>;<arg>;...m"` to the stream. There's no `';'` after the last arg, but `'m'`.
