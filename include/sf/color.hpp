//StreamFormat color.hpp
#ifndef SF_COLOR_HPP
#define SF_COLOR_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX17) || _MSC_VER >= 1910

#include <sf/ansi.hpp>
#include <variant>

namespace sf
{
    enum preset_color : unsigned char
    {
        black = 30,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white,
        extendend,
        user_default, //To make clang happy, use `user_default` instead of `default` keyword.
        bright_black = 90,
        bright_red,
        bright_green,
        bright_yellow,
        bright_blue,
        bright_magenta,
        bright_cyan,
        bright_white
    };

    struct rgb_color
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        rgb_color() = default;
        SF_CONSTEXPR rgb_color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
    };

    namespace internal
    {
        class color
        {
        public:
            typedef std::variant<preset_color, unsigned char, rgb_color> color_type;

        private:
            color_type value;
            bool isback;

        public:
            color() = default;
            color(color_type value, bool isback) : value(value), isback(isback) {}
            template <typename Char, typename Traits>
            friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color& c)
            {
                switch (c.value.index())
                {
                case 0://preset_color
                    stream << (static_cast<int>(std::get<preset_color>(c.value)) + (c.isback ? 10 : 0));
                    break;
                case 1://unsigned char
                    stream << (c.isback ? 48 : 38) << smcolon<Char>() << 5 << smcolon<Char>() << static_cast<int>(std::get<unsigned char>(c.value));
                    break;
                case 2://rgb_color
                    rgb_color rgb = std::get<rgb_color>(c.value);
                    stream << (c.isback ? 48 : 38) << smcolon<Char>() << 2
                           << smcolon<Char>() << static_cast<int>(rgb.r)
                           << smcolon<Char>() << static_cast<int>(rgb.g)
                           << smcolon<Char>() << static_cast<int>(rgb.b);
                    break;
                }
                return stream;
            }
        };

        //Pack an arg with its foreground and background color.
        template <typename T>
        class color_arg
        {
        public:
            typedef color::color_type color_type;

        private:
            T arg;
            color fore, back;

        public:
            SF_CONSTEXPR color_arg() : fore(user_default, false), back(user_default, true) {}
            SF_CONSTEXPR color_arg(T&& arg, color_type fore, color_type back) : arg(arg), fore(fore, false), back(back, true) {}
            template <typename Char, typename Traits = std::char_traits<Char>>
            friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color_arg<T>& arg)
            {
                return stream << make_ansi_control(arg.fore, arg.back) << arg.arg << make_ansi_control();
            }
        };
    } // namespace internal

    using color_type = typename internal::color::color_type;

    template <typename T>
    SF_CONSTEXPR internal::color_arg<T> make_color_arg(T&& arg, color_type fore, color_type back = user_default)
    {
        return internal::color_arg<T>(std::forward<T>(arg), fore, back);
    }
} // namespace sf

#endif // !SF_CXX17

#endif // !SF_COLOR_HPP
