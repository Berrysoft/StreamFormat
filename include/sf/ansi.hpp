/**StreamFormat ansi.hpp
 * 
 * MIT License
 * 
 * Copyright (c) 2018 Berrysoft
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
#ifndef SF_ANSI_HPP
#define SF_ANSI_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX11)

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

        template <typename Char, typename Traits>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args(std::basic_ostream<Char, Traits>& stream)
        {
            return stream;
        }
        template <typename Char, typename Traits, typename Arg0>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args(std::basic_ostream<Char, Traits>& stream, Arg0&& arg0)
        {
            return stream << std::forward<Arg0>(arg0);
        }
        template <typename Char, typename Traits, typename Arg0, typename... Args>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args(std::basic_ostream<Char, Traits>& stream, Arg0&& arg0, Args&&... args)
        {
            return join_args(stream << std::forward<Arg0>(arg0) << smcolon<Char>(), std::forward<Args>(args)...);
        }

        template <std::size_t... Indices>
        struct index_sequence
        {
        };

        template <std::size_t N, typename = void>
        struct make_index_sequence_impl
        {
            template <typename>
            struct tmp;

            template <std::size_t... Prev>
            struct tmp<index_sequence<Prev...>>
            {
                using type = index_sequence<Prev..., N - 1>;
            };

            using type = typename tmp<typename make_index_sequence_impl<N - 1>::type>::type;
        };
        template <std::size_t N>
        struct make_index_sequence_impl<N, typename std::enable_if<N == 0>::type>
        {
            using type = index_sequence<>;
        };

        template <std::size_t N>
        using make_index_sequence = typename make_index_sequence_impl<N>::type;

        template <typename Char, typename Traits, typename... Args, std::size_t... Indices>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& join_args_helper(std::basic_ostream<Char, Traits>& stream, const std::tuple<Args...>& args, index_sequence<Indices...>)
        {
            return join_args(stream, std::get<Indices>(args)...);
        }

        template <typename Char, typename Traits, typename... Args>
        SF_CONSTEXPR std::basic_ostream<Char, Traits>& write_ansi(std::basic_ostream<Char, Traits>& stream, const std::tuple<Args...>& args)
        {
            return join_args_helper(stream << esc<Char>() << sqr_bra<Char>(), args, make_index_sequence<sizeof...(Args)>()) << ansi_end<Char>();
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
