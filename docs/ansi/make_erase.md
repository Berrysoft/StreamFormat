# `sf::make_erase_*`
Defined in [`<sf/ansi.hpp>`](./index.md).
``` c++
enum erase_opt
{
    erase_to_end,
    erase_to_start,
    erase_all,
    erase_all_buffer
};

// 1
template <
    typename Char
> constexpr /*unspecified*/ make_erase_screen(erase_opt opt);

// 2
template <
    typename Char
> constexpr /*unspecified*/ make_erase_line(erase_opt opt);
```

1. Erase the whole screen.
2. Erase the current line.

Both functions erase by the following options:

|Element|Summary|
|-|-|
|`erase_to_end`|Erase form current to the end.|
|`erase_to_start`|Erase form current to the start.|
|`erase_all`|Erase all.|
|`erase_all_buffer`|Erase all and erase all buffers, only apply to `make_erase_screen`.|
