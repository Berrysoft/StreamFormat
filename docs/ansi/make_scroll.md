# `sf::make_scroll_*`
Defined in [`<sf/ansi.hpp`](./index.md).
``` c++
// 1
template <
    typename Char
> constexpr /*unspecified*/ make_scroll_up(std::size_t n = 1);

// 2
template <
    typename Char
> constexpr /*unspecified*/ make_scroll_down(std::size_t n = 1);
```

1. Scroll the whole page up by `n` lines.
1. Scroll the whole page down by `n` lines.
