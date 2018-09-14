// StreamFormat ansi.hpp
#ifndef SF_ANSI_HPP
#define SF_ANSI_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX11) || _MSC_VER >= 1900

#include <ostream>
#include <tuple>

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
            return stream << esc<Char>() << sqr_bra<Char>() << 0 << ansi_end<Char>();
        }
    } // namespace internal

    //Write Console Virtual Terminal Sequences (ANSI Control Characters) to a stream.
    template <typename... Args>
    class ansi_control
    {
    private:
        std::tuple<Args...> args;

    public:
        ansi_control(Args&&... args) : args(std::forward<Args>(args)...) {}
        template <typename Char, typename Traits = std::char_traits<Char>>
        friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const ansi_control& ctrl)
        {
            return internal::write_ansi(stream, ctrl.args);
        }
    };

    template <typename... Args>
    SF_CONSTEXPR ansi_control<Args...> make_ansi_control(Args&&... args)
    {
        return ansi_control<Args...>(std::forward<Args>(args)...);
    }

    enum color : int
    {
        black = 0,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white,
        extendend,
        user_default //To make clang happy, use `user_default` instead of `default` keyword.
    };
    template <typename T>
    class color_arg
    {
    private:
        T arg;
        int fc, bc;

    public:
        SF_CONSTEXPR color_arg() = default;
        SF_CONSTEXPR color_arg(T&& arg, color fore, bool foreb, color back, bool backb) : arg(arg)
        {
            fc = foreb ? fore + 90 : fore + 30;
            bc = backb ? back + 100 : back + 40;
        }
        template <typename Char, typename Traits = std::char_traits<Char>>
        friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color_arg<T>& arg)
        {
            return stream << make_ansi_control(arg.fc, arg.bc) << arg.arg << make_ansi_control();
        }
    };

    //To make compilers happy, use make_color_arg instead of color_arg, please.
    template <typename T>
    SF_CONSTEXPR color_arg<T> make_color_arg(T&& arg, color fore, bool foreb = false)
    {
        return color_arg<T>(std::forward<T>(arg), fore, foreb, user_default, false);
    }
    template <typename T>
    SF_CONSTEXPR color_arg<T> make_color_arg(T&& arg, color fore, color back)
    {
        return color_arg<T>(std::forward<T>(arg), fore, false, back, false);
    }
    template <typename T>
    SF_CONSTEXPR color_arg<T> make_color_arg(T&& arg, color fore, bool foreb, color back, bool backb)
    {
        return color_arg<T>(std::forward<T>(arg), fore, foreb, back, backb);
    }
} // namespace sf

#endif // SF_CXX11

#endif // !SF_ANSI_HPP
