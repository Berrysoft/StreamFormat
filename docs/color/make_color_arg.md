# `sf::make_color_arg`
Defined in [`<sf/color.hpp>`](./index.md).
``` c++
typedef /*unspecified*/ color_type;

// 1
template <
    typename T
> constexpr /*unspecified*/ make_color_arg(T&& arg, color_type fore, color_type back = user_default, sgr_chars sgr = normal);

// 2
template <
    typename T
> constexpr /*unspecified*/ make_color_arg(T&& arg, sgr_chars sgr);
```

|Param|Summary|
|-|-|
|`arg`|Arg to output.|
|`fore`|Foreground.|
|`back`|Background.|
|`sgr`|SGR command.|

`color_type` is a special designed type, and it accepts `preset_color`, `unsigned char` and `rgb_color`.

When using Windows with version below Windows 10 TH2, the CMD doesn't support ANSI escape codes. You can `#define SF_WIN_NATIVE_COLOR` to use Windows native functions.
