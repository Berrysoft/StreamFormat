/**StreamFormat ansi.hpp
 * 
 * MIT License
 * 
 * Copyright (c) 2018-2020 Berrysoft
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

#include <ostream>
#include <tuple>

namespace sf
{
    namespace internal
    {
        template <typename Char, typename Traits, typename Arg0, typename... Args>
        constexpr std::basic_ostream<Char, Traits>& join_args(std::basic_ostream<Char, Traits>& stream, Arg0&& arg0, Args&&... args)
        {
            if constexpr (sizeof...(Args) > 0)
                return join_args(stream << std::forward<Arg0>(arg0) << Char{ ';' }, std::forward<Args>(args)...);
            else
                return stream << std::forward<Arg0>(arg0);
        }

        template <typename Char, typename Traits, typename... Args, std::size_t... Indeces>
        constexpr std::basic_ostream<Char, Traits>& join_args_helper(std::basic_ostream<Char, Traits>& stream, const std::tuple<Args...>& args, std::index_sequence<Indeces...>)
        {
            return join_args(stream, std::get<Indeces>(args)...);
        }

        template <typename Char, typename Traits, typename... Args>
        constexpr std::basic_ostream<Char, Traits>& write_ansi(std::basic_ostream<Char, Traits>& stream, const std::tuple<Args...>& args)
        {
            stream << Char{ '\033' } << Char{ '[' };
            if constexpr (sizeof...(Args) > 0)
                return join_args_helper(stream, args, std::make_index_sequence<sizeof...(Args)>{});
            else
                return stream;
        }

        //Write Console Virtual Terminal Sequences (ANSI Control Characters) to a stream.
        template <typename C, typename... Args>
        class ansi_control
        {
        private:
            C endc;
            std::tuple<Args...> args;

        public:
            constexpr ansi_control(C endc, Args&&... args) noexcept : endc(endc), args(std::forward<Args>(args)...) {}
            template <typename Char, typename Traits = std::char_traits<Char>>
            friend constexpr std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const ansi_control& ctrl)
            {
                return write_ansi(stream, ctrl.args) << ctrl.endc;
            }
        };

        template <typename Char, typename... Args>
        constexpr ansi_control<Char, Args...> make_ansi_control(Char endc, Args&&... args)
        {
            return ansi_control<Char, Args...>(endc, std::forward<Args>(args)...);
        }
    } // namespace internal

#define SF_MAKE_MOVE(name, c)                                                            \
    template <typename Char>                                                             \
    constexpr internal::ansi_control<Char, std::size_t> name(std::size_t n = 1) noexcept \
    {                                                                                    \
        return internal::make_ansi_control(Char{ c }, std::move(n));                     \
    }

    SF_MAKE_MOVE(make_cursor_upward, 'A')
    SF_MAKE_MOVE(make_cursor_downward, 'B')
    SF_MAKE_MOVE(make_cursor_forward, 'C')
    SF_MAKE_MOVE(make_cursor_backward, 'D')

    SF_MAKE_MOVE(make_cursor_next_line, 'E')
    SF_MAKE_MOVE(make_cursor_pre_line, 'F')
    SF_MAKE_MOVE(make_cursor_abs_line, 'G')

    template <typename Char>
    constexpr internal::ansi_control<Char, std::size_t, std::size_t> make_cursor_set_pos(std::size_t line = 1, std::size_t index = 1) noexcept
    {
        return internal::make_ansi_control(Char{ 'H' }, std::move(line), std::move(index));
    }

    enum erase_opt
    {
        erase_to_end,
        erase_to_start,
        erase_all,
        erase_all_buffer // Only apply to ED.
    };

    template <typename Char>
    constexpr internal::ansi_control<Char, int> make_erase_screen(erase_opt opt) noexcept
    {
        return internal::make_ansi_control(Char{ 'J' }, static_cast<int>(opt));
    }

    template <typename Char>
    constexpr internal::ansi_control<Char, int> make_erase_line(erase_opt opt) noexcept
    {
        return internal::make_ansi_control(Char{ 'K' }, static_cast<int>(opt));
    }

    SF_MAKE_MOVE(make_scroll_up, 'S')
    SF_MAKE_MOVE(make_scroll_down, 'T')

    template <typename Char, typename... Args>
    constexpr internal::ansi_control<Char, Args...> make_sgr_control(Args&&... args) noexcept
    {
        return internal::make_ansi_control(Char{ 'm' }, std::forward<Args>(args)...);
    }

    template <typename Char>
    constexpr internal::ansi_control<Char, int> make_cursor_pos_report() noexcept
    {
        return internal::make_ansi_control(Char{ 'n' }, 6);
    }

    template <typename Char>
    constexpr internal::ansi_control<Char> make_cursor_save() noexcept
    {
        return internal::make_ansi_control(Char{ 's' });
    }
    template <typename Char>
    constexpr internal::ansi_control<Char> make_cursor_restore() noexcept
    {
        return internal::make_ansi_control(Char{ 'u' });
    }
} // namespace sf

#endif // !SF_ANSI_HPP
