# `sf::make_sgr_control`
Defined in [`<sf/ansi.hpp>`](./index.md)
``` c++
template <
    typename Char,
    typename... Args
> constexpr /*unspecified*/ make_sgr_control(Args&&... args);
```

Output `args...` between `\033[` and `m`.
