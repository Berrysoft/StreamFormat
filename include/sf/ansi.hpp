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
        template <typename Char,typename Traits>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args(std::basic_ostream<Char, Traits>& stream)
        {
            return stream;
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
            join_args_helper(stream, args, std::make_index_sequence<sizeof...(Args)>());
            stream << ansi_end<Char>();
            return stream;
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
} // namespace sf

#endif // SF_CXX11

#endif // !SF_ANSI_HPP
