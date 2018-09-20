// StreamFormat ansi.hpp
#ifndef SF_ANSI_HPP
#define SF_ANSI_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX11) || _MSC_VER >= 1910

#include <ostream>
#include <tuple>
#include <variant>

namespace sf
{
    namespace internal
    {
        SF_CHAR_TEMPLATE(esc, '\033')
        SF_CHAR_TEMPLATE(sqr_bra, '[')
        SF_CHAR_TEMPLATE(smcolon, ';')
        SF_CHAR_TEMPLATE(ansi_end, 'm')

        template <typename Char, typename Traits, typename Arg0, typename... Args>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args(std::basic_ostream<Char, Traits>& stream, Arg0&& arg0, Args&&... args)
        {
            return join_args(stream << std::forward<Arg0>(arg0) << smcolon<Char>(), std::forward<Args>(args)...);
        }
        template <typename Char, typename Traits, typename Arg0>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args(std::basic_ostream<Char, Traits>& stream, Arg0&& arg0)
        {
            return stream << std::forward<Arg0>(arg0);
        }
        template <typename Char, typename Traits, typename... Args, std::size_t... Indices>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args_helper(std::basic_ostream<Char, Traits>& stream, const std::tuple<Args...>& args, std::index_sequence<Indices...>)
        {
            return join_args(stream, std::forward<Args>(std::get<Indices>(args))...);
        }

        template <typename Char, typename Traits, typename... Args>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& write_ansi(std::basic_ostream<Char, Traits>& stream, const std::tuple<Args...>& args)
        {
            stream << esc<Char>() << sqr_bra<Char>();
            join_args_helper(stream, args, std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>());
            stream << ansi_end<Char>();
            return stream;
        }
        template <typename Char, typename Traits>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& write_ansi(std::basic_ostream<Char, Traits>& stream, const std::tuple<>& args)
        {
            return stream << esc<Char>() << sqr_bra<Char>() << ansi_end<Char>();
        }

        //Write Console Virtual Terminal Sequences (ANSI Control Characters) to a stream.
        template <typename... Args>
        class ansi_control
        {
        private:
            std::tuple<Args...> args;

        public:
            ansi_control(Args&&... args) : args(std::forward<Args>(args)...) {}
            SF_CONSTEXPR ansi_control(ansi_control<Args...>&& arg) : args(std::move(arg.args)) {}
            ansi_control<Args...>& operator=(ansi_control<Args...>&& arg)
            {
                args = std::move(arg.args);
                return *this;
            }
            template <typename Char, typename Traits = std::char_traits<Char>>
            friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const ansi_control& ctrl)
            {
                return internal::write_ansi(stream, ctrl.args);
            }
        };
    } // namespace internal

    template <typename... Args>
    SF_CONSTEXPR internal::ansi_control<Args...> make_ansi_control(Args&&... args)
    {
        return internal::ansi_control<Args...>(std::forward<Args>(args)...);
    }

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
            template <typename Char, typename Traits = std::char_traits<Char>>
            friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color& c)
            {
                switch (c.value.index())
                {
                case 0:
                    stream << (static_cast<int>(std::get<preset_color>(c.value)) + (c.isback ? 10 : 0));
                    break;
                case 1:
                    stream << (c.isback ? 48 : 38) << smcolon<Char>() << 5 << smcolon<Char>() << static_cast<int>(std::get<unsigned char>(c.value));
                    break;
                case 2:
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

#endif // SF_CXX11

#endif // !SF_ANSI_HPP
