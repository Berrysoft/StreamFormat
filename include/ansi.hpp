#ifndef SF_ANSI_HPP
#define SF_ANSI_HPP

#include "utility.hpp"

#if defined(SF_CXX14) || _MSC_VER >= 1900

#include <initializer_list>
#include <ostream>

namespace sf
{
    namespace internal
    {
        SF_CHAR_TEMPLATE(esc, '\033')
        SF_CHAR_TEMPLATE(sqr_bra, '[')
        SF_CHAR_TEMPLATE(smcolon, ';')
        SF_CHAR_TEMPLATE(ansi_end, 'm')

        template <typename Char, typename Traits = std::char_traits<Char>>
        std::basic_ostream<Char, Traits>& write_ansi(std::basic_ostream<Char, Traits>& stream, const std::initializer_list<int>& list)
        {
            stream << esc<Char>() << sqr_bra<Char>();
            if (list.size())
            {
                auto it = list.begin();
                stream << *it;
                for (; it != list.end(); it++)
                {
                    stream << smcolon<Char>() << *it;
                }
            }
            else
            {
                stream << 0;
            }
            stream << ansi_end<Char>();
            return stream;
        }
    } // namespace internal

    //Write Console Virtual Terminal Sequences (ANSI Control Characters) to a stream.
    class ansi_control
    {
    private:
        std::initializer_list<int> list;

    public:
        ansi_control() : list() {}
        ansi_control(std::initializer_list<int> list) : list(list) {}
        template <typename Char, typename Traits = std::char_traits<Char>>
        friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const ansi_control& ctrl)
        {
            return internal::write_ansi(stream, ctrl.list);
        }
    };

    enum color_value : int
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
        user_default //Make compilers happy.
    };
    template <typename T>
    class color_arg
    {
    private:
        T arg;
        int fc, bc;

    public:
        SF_CONSTEXPR color() = default;
        SF_CONSTEXPR color(T&& arg, color_value fore, bool foreb, color_value back, bool backb) : arg(arg)
        {
            fc = foreb ? fore + 90 : fore + 30;
            bc = backb ? back + 100 : back + 40;
        }
        template <typename Char, typename Traits = std::char_traits<Char>>
        friend SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const color_arg<T>& arg)
        {
            return stream << ansi_control{ arg.fc, arg.bc } << arg.arg << ansi_control{};
        }
    };

    //To make compilers happy, use make_color_arg instead of color_arg, please.
    template <typename T>
    SF_CONSTEXPR color_arg<T> make_color_arg(T&& arg, color fore, bool foreb = false)
    {
        return color_arg<T>(static_cast<T&&>(arg), fore, foreb, user_default, false);
    }
    template <typename T>
    SF_CONSTEXPR color_arg<T> make_color_arg(T&& arg, color fore, color back)
    {
        return color_arg<T>(static_cast<T&&>(arg), fore, false, back, false);
    }
    template <typename T>
    SF_CONSTEXPR color_arg<T> make_color_arg(T&& arg, color fore, bool foreb, color back, bool backb)
    {
        return color_arg<T>(static_cast<T&&>(arg), fore, foreb, back, backb);
    }
} // namespace sf

#endif // SF_CXX14

#endif // !SF_ANSI_HPP
