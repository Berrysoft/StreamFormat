# `sf::make_cursor_*`
Defined in [`<sf/ansi.hpp>`](./index.md).
``` c++
// 1
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_upward(std::size_t n = 1);

// 2
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_downward(std::size_t n = 1);

// 3
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_forward(std::size_t n = 1);

// 4
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_backward(std::size_t n = 1);

// 5
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_next_line(std::size_t n = 1);

// 6
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_pre_line(std::size_t n = 1);

// 7
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_abs_line(std::size_t n = 1);

// 8
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_set_pos(std::size_t line = 1, std::size_t index = 1);

// 9
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_pos_report();

// 10
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_save();

// 11
template <
    typename Char
> constexpr /*unspecified*/ make_cursor_restore();
```

1. Move cursor upward `n` cells.
2. Move cursor downward `n` cells.
3. Move cursor forward `n` cells.
4. Move cursor backward `n` cells.
5. Move cursor `n` lines down.
6. Move cursor `n` lines up.
7. Move cursor to line `n`.
8. Move cursor to line `line`, index `index`.
9. Send a DSR(Device Status Report), and get the cursor position from stdin as `\033[n;mR`, where `n` is the line and `m` is the index.
10. Save the current cursor position.
11. Restore the save cursor position.
