# StreamFormat
StreamFormat is a template library, thus it contains only headers. Here are all the **public** headers(in which the APIs won't change occasionally):

|Header|Contents|
|-|-|
|[`<sf/format.hpp>`](./format/index.md)|IO functions.|
|[`<sf/sformat.hpp>`](./sformat/index.md)|Format IO functions for `std::basic_string`.|
|[`<sf/ansi.hpp>`](./ansi/index.md)|A function to write ANSI escape code.|
|[`<sf/color.hpp>`](./color/index.md)|Classes and functions to output colorfully.|

## Global control
|Macro|Summery|
|-|-|
|`SF_USE_NO_EXCEPT`|Define and all the functions won't throw.|
|`SF_FORCE_WIDE_IO`|Define and will force some `print`-like functions use wide edition.|
|`SF_WIN_NATIVE_COLOR`|Define and use native functions to control colors on Windows.|
