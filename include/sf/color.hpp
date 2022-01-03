/**StreamFormat color.hpp
 * 
 * MIT License
 * 
 * Copyright (c) 2018-2022 Berrysoft
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */
#ifndef SF_COLOR_HPP
#define SF_COLOR_HPP

#include <sf/utility.hpp>

#include <sf/ansi.hpp>
#include <variant>

namespace sf
{
    enum sgr_chars : std::uint8_t
    {
        normal,
        bold,
        faint,
        italic,
        underline,
        blink,
        rapid,
        reverse_video,
        conceal,
        crossed,
        primary,
        fraktur = 20,
        double_underline,
        bold_off = 21,
        intensity,
        italic_off,
        fraktur_off = 23,
        underline_off,
        blink_off,
        inverse_off = 27,
        conceal_off,
        crossed_off,
        foreground = 38,
        background = 48,
        framed = 51,
        encircled,
        overlined,
        framed_off,
        encircled_off = 54,
        overlined_off
    };

    enum preset_color : std::uint8_t
    {
        black = 30,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white,
        user_default = 39, //To make clang happy, use `user_default` instead of `default` keyword.
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
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
    };

    namespace internal
    {
        using color_type = std::variant<preset_color, std::uint8_t, rgb_color>;
        class color
        {
        private:
            color_type value;
            bool isback;

        public:
            constexpr color() noexcept : value(), isback() {}
            constexpr color(color_type value, bool isback) noexcept(std::is_nothrow_copy_constructible_v<color_type>) : value(value), isback(isback) {}
            template <typename Char, typename Traits>
            friend constexpr std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color& c)
            {
                switch (c.value.index())
                {
                case 0: //preset_color
                    stream << (static_cast<int>(std::get<preset_color>(c.value)) + (c.isback ? 10 : 0));
                    break;
                case 1: //unsigned char
                    join_args(stream, static_cast<int>(c.isback ? background : foreground), 5, static_cast<int>(std::get<std::uint8_t>(c.value)));
                    break;
                case 2: //rgb_color
                    rgb_color rgb = std::get<rgb_color>(c.value);
                    join_args(stream, static_cast<int>(c.isback ? background : foreground), 2, static_cast<int>(rgb.r), static_cast<int>(rgb.g), static_cast<int>(rgb.b));
                    break;
                }
                return stream;
            }
        };

        //Pack an arg with its foreground and background color.
        template <typename T>
        class color_arg
        {
        private:
            T arg;
            color fore, back;
            sgr_chars sgr;

        public:
            constexpr color_arg() noexcept : fore(user_default, false), back(user_default, true), sgr(normal) {}
            constexpr color_arg(T&& arg, color_type fore, color_type back, sgr_chars sgr) noexcept : arg(arg), fore(fore, false), back(back, true), sgr(sgr) {}
            template <typename Char, typename Traits>
            friend constexpr std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color_arg& arg)
            {
                return stream << make_sgr_control<Char>(static_cast<int>(arg.sgr), arg.fore, arg.back) << arg.arg << make_sgr_control<Char>();
            }
        };
    } // namespace internal

    using color_type = internal::color_type;

    template <typename T>
    constexpr internal::color_arg<T> make_color_arg(T&& arg, color_type fore, color_type back = user_default, sgr_chars sgr = normal) noexcept
    {
        return internal::color_arg<T>(std::forward<T>(arg), fore, back, sgr);
    }
    template <typename T>
    constexpr internal::color_arg<T> make_color_arg(T&& arg, sgr_chars sgr) noexcept
    {
        return internal::color_arg<T>(std::forward<T>(arg), user_default, user_default, sgr);
    }
} // namespace sf

#endif // !SF_COLOR_HPP
