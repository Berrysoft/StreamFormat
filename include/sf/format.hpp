/**StreamFormat format.hpp
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
#ifndef SF_FORMAT_HPP
#define SF_FORMAT_HPP

#include <sf/utility.hpp>

#include <functional>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

namespace sf
{
    namespace internal
    {
        //For convinence.
        enum io_state : bool
        {
            input,
            output
        };

        //Define types.
        template <io_state IOState, typename Char, typename Traits>
        struct stream;
        template <typename Char, typename Traits>
        struct stream<input, Char, Traits>
        {
            using type = std::basic_istream<Char, Traits>;
        };
        template <typename Char, typename Traits>
        struct stream<output, Char, Traits>
        {
            using type = std::basic_ostream<Char, Traits>;
        };

        template <io_state IOState, typename Char, typename Traits>
        using stream_t = typename stream<IOState, Char, Traits>::type;

        template <typename Stream>
        struct arg
        {
            using type = std::function<Stream&(Stream&)>;
            using list_type = std::vector<type>;
        };

        template <typename Stream>
        using arg_t = typename arg<Stream>::type;
        template <typename Stream>
        using arg_list_t = typename arg<Stream>::list_type;

        //A packed arg.
        template <io_state IOState, typename T, typename Char, typename Traits>
        class arg_io
        {
        public:
            using stream_type = stream_t<IOState, Char, Traits>;

        private:
            T arg;

        public:
            arg_io(T&& arg) noexcept(std::is_nothrow_move_constructible_v<T>) : arg(std::forward<T>(arg)) {}
            constexpr stream_type& operator()(stream_type& stream)
            {
                if constexpr (IOState == input)
                    return stream >> arg;
                else
                    return stream << arg;
            }
        };

        template <typename Int, typename Char, typename Traits>
        constexpr Int stou(std::basic_string_view<Char, Traits> str) noexcept
        {
            Int result(0);
            for (Char c : str)
            {
                result *= 10;
                result += c - Char{ '0' };
            }
            return result;
        }

        //Input/Output string slice.
        template <io_state IOState, typename Char, typename Traits>
        class string_view_io
        {
        public:
            typedef stream_t<IOState, Char, Traits> stream_type;
            typedef std::basic_string_view<Char, Traits> string_view_type;

        private:
            string_view_type arg;

        public:
            constexpr string_view_io(string_view_type arg) : arg(arg) {}
            stream_type& operator()(stream_type& stream)
            {
                if constexpr (IOState == io_state::input)
                {
                    if (stream)
                    {
                        for (const Char& c : arg)
                        {
                            if (Traits::eq(c, Char{ ' ' }))
                            {
                                while (true)
                                {
                                    auto t = stream.peek();
                                    if (t != Traits::eof() &&
                                        (Traits::eq(t, Char{ ' ' }) || Traits::eq(t, Char{ '\t' }) || Traits::eq(t, Char{ '\v' }) || Traits::eq(t, Char{ '\r' }) || Traits::eq(t, Char{ '\n' })))
                                    {
                                        stream.get();
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                while (true)
                                {
                                    auto t = stream.get();
                                    if (t == Traits::eof() || Traits::eq(t, c))
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    return stream;
                }
                else
                {
                    return stream << arg;
                }
            }
        };

        template <io_state IOState, typename Char, typename Traits>
        std::ios_base::fmtflags stream_setf_w(std::ios_base::fmtflags Flag, std::ios_base::fmtflags Base, Char Fill, stream_t<IOState, Char, Traits>& stream, int fmtf)
        {
            std::ios_base::fmtflags oldf = stream.setf(Flag, Base);
            stream.fill(Fill);
            stream.width(fmtf);
            return oldf;
        }
        template <io_state IOState, typename Char, typename Traits>
        std::ios_base::fmtflags stream_setf_f(std::ios_base::fmtflags Flag, stream_t<IOState, Char, Traits>& stream, int)
        {
            return stream.setf(Flag);
        }
        template <io_state IOState, typename Char, typename Traits>
        std::ios_base::fmtflags stream_setf_p(std::ios_base::fmtflags Flag, std::ios_base::fmtflags Base, stream_t<IOState, Char, Traits>& stream, int fmtf)
        {
            std::ios_base::fmtflags oldf = stream.setf(Flag, Base);
            stream.precision(fmtf);
            return oldf;
        }

        template <io_state IOState, typename Char, typename Traits>
        std::ios_base::fmtflags stream_setf(Char fmtc, stream_t<IOState, Char, Traits>& stream, int fmtf)
        {
            switch (fmtc)
            {
            case Char{ 'd' }:
                return stream_setf_w<IOState, Char, Traits>(std::ios_base::dec, std::ios_base::basefield, Char{ '0' }, stream, fmtf);
            case Char{ 'o' }:
                return stream_setf_w<IOState, Char, Traits>(std::ios_base::oct, std::ios_base::basefield, Char{ '0' }, stream, fmtf);
            case Char{ 'x' }:
                return stream_setf_w<IOState, Char, Traits>(std::ios_base::hex, std::ios_base::basefield, Char{ '0' }, stream, fmtf);
            case Char{ 'e' }:
                return stream_setf_p<IOState, Char, Traits>(std::ios_base::scientific, std::ios_base::floatfield, stream, fmtf);
            case Char{ 'f' }:
                return stream_setf_p<IOState, Char, Traits>(std::ios_base::fixed, std::ios_base::floatfield, stream, fmtf);
            case Char{ 'l' }:
                return stream_setf_w<IOState, Char, Traits>(std::ios_base::left, std::ios_base::adjustfield, Char{ ' ' }, stream, fmtf);
            case Char{ 'r' }:
                return stream_setf_w<IOState, Char, Traits>(std::ios_base::right, std::ios_base::adjustfield, Char{ ' ' }, stream, fmtf);
            case Char{ 'i' }:
                return stream_setf_w<IOState, Char, Traits>(std::ios_base::internal, std::ios_base::adjustfield, Char{ ' ' }, stream, fmtf);
            case Char{ 'b' }:
                return stream_setf_f<IOState, Char, Traits>(std::ios_base::boolalpha, stream, fmtf);
            case Char{ 'u' }:
                return stream_setf_f<IOState, Char, Traits>(std::ios_base::uppercase, stream, fmtf);
            case Char{ 's' }:
                return stream_setf_f<IOState, Char, Traits>(std::ios_base::showbase, stream, fmtf);
            case Char{ 'g' }:
            default:
                return static_cast<std::ios_base::fmtflags>(0);
            }
        }

        template <io_state IOState, typename Char, typename Traits>
        class format_arg_io
        {
        public:
            using stream_type = stream_t<IOState, Char, Traits>;
            using arg_type = arg_t<stream_type>;
            using string_view_type = std::basic_string_view<Char, Traits>;
            using int_type = typename string_view_type::size_type;

        private:
            arg_type& ori;
            string_view_type fmts;

        public:
            constexpr format_arg_io(arg_type& ori, string_view_type fmts) noexcept : ori(ori), fmts(fmts) {}
            constexpr stream_type& operator()(stream_type& stream)
            {
                std::ios_base::fmtflags oldf{};
                int_type length = fmts.length();
                int_type offset = 0, index = 0;
                for (; index <= length; index++)
                {
                    if (index == length || Traits::eq(fmts[index], Char{ ',' }))
                    {
                        Char fmtc = fmts[offset];
                        int fmtf = 0;
                        int_type len = index - offset - 1;
                        if (len > 0)
                        {
                            fmtf = stou<int, Char, Traits>(fmts.substr(offset + 1, len));
                        }
                        oldf |= stream_setf<IOState, Char, Traits>(fmtc, stream, fmtf);
                        offset = index + 1;
                    }
                }
                ori(stream);
                if (oldf)
                    stream.setf(oldf);
                return stream;
            }
        };

        //A pack of format string and arguments.
        template <io_state IOState, typename Char, typename Traits>
        class format_string_view
        {
        public:
            using stream_type = stream_t<IOState, Char, Traits>;
            using arg_type = arg_t<stream_type>;
            using arg_list_type = arg_list_t<stream_type>;
            using string_view_type = std::basic_string_view<Char, Traits>;
            using int_type = typename string_view_type::size_type;
            using string_view_io_type = string_view_io<IOState, Char, Traits>;

        private:
            string_view_type fmt;
            arg_list_type args;

        public:
            constexpr format_string_view(string_view_type fmt, arg_list_type&& args) noexcept : fmt(fmt), args(std::move(args))
            {
            }
            constexpr stream_type& operator()(stream_type& stream)
            {
                int_type offset = 0, index = 0;
                const int_type length = fmt.length();
                bool in_number = false;
                std::size_t arg_index = 0;
                while (offset < length)
                {
                    if (!in_number)
                    {
                        int_type off = offset;
                        for (index = offset; index < length; index++)
                        {
                            if (Traits::eq(fmt[index], Char{ '{' }))
                            {
                                index++;
                                if (!(index < length && Traits::eq(fmt[index], Char{ '{' })))
                                {
                                    in_number = true;
                                    index--;
                                }
                            }
                            else if (Traits::eq(fmt[index], Char{ '}' }))
                            {
                                index++;
                                if (!(index < length && Traits::eq(fmt[index], Char{ '}' })))
                                {
                                    index--;
                                    continue;
                                }
                            }
                            else
                            {
                                continue;
                            }
                            break;
                        }
                        int_type len = index - offset;
                        offset = index + 1;
                        if (len <= 0)
                            continue;
                        string_view_io_type{ fmt.substr(off, len) }(stream);
                    }
                    else
                    {
                        for (index = offset; index < length; index++)
                        {
                            if (Traits::eq(fmt[index], Char{ '}' }))
                                break;
                        }
                        if (index == length)
                        {
                            string_view_io_type(fmt.substr(offset - 1))(stream);
                            offset = index + 1;
                        }
                        else
                        {
                            in_number = false;
                            int_type ci;
                            for (ci = offset; ci < index; ci++)
                            {
                                if (Traits::eq(fmt[ci], Char{ ':' }))
                                {
                                    break;
                                }
                            }
                            if (ci > offset)
                            {
                                arg_index = stou<std::size_t, Char, Traits>(fmt.substr(offset, ci - offset));
                            }
                            if (arg_index >= args.size())
                            {
                                string_view_io_type{ fmt.substr(offset - 1, index - offset + 2) }(stream);
                            }
                            else if (index == ci)
                            {
                                args[arg_index](stream);
                            }
                            else
                            {
                                format_arg_io<IOState, Char, Traits>{ args[arg_index], fmt.substr(ci + 1, index - ci - 1) }(stream);
                            }
                            offset = index + 1;
                            arg_index++;
                        }
                    }
                }
                return stream;
            }
        };

        template <io_state IOState, typename Char, typename Traits>
        constexpr stream_t<IOState, Char, Traits>& vformat(stream_t<IOState, Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, arg_list_t<stream_t<IOState, Char, Traits>>&& args)
        {
            return format_string_view<IOState, Char, Traits>{ fmt, std::move(args) }(stream);
        }

        template <io_state IOState, typename Char, typename Traits, typename... Args>
        constexpr stream_t<IOState, Char, Traits>& format(stream_t<IOState, Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args)
        {
            return vformat<IOState, Char, Traits>(stream, fmt, arg_list_t<stream_t<IOState, Char, Traits>>{ arg_io<IOState, Args, Char, Traits>(std::forward<Args>(args))... });
        }

        template <io_state IOState, typename Char, typename Traits, typename T>
        constexpr stream_t<IOState, Char, Traits>& put(stream_t<IOState, Char, Traits>& stream, T&& arg)
        {
            return arg_io<IOState, T, Char, Traits>{ std::forward<T>(arg) }(stream);
        }
    } // namespace internal

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename String, typename... Args, typename = std::enable_if_t<std::is_convertible_v<String, std::basic_string_view<Char, Traits>>>>
    constexpr std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, String&& fmt, Args&&... args)
    {
        return internal::format<internal::input, Char, Traits>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename T>
    constexpr std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, T&& arg)
    {
        return internal::put<internal::input, Char, Traits>(stream, std::forward<T>(arg));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename String, typename = std::enable_if_t<std::is_convertible_v<String, std::basic_string_view<Char, Traits>>>>
    constexpr std::basic_istream<Char, Traits>& vscan(std::basic_istream<Char, Traits>& stream, String&& fmt, internal::arg_list_t<internal::stream_t<internal::input, Char, Traits>>&& args)
    {
        return internal::vformat<internal::input, Char, Traits>(stream, fmt, std::move(args));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename String, typename... Args, typename = std::enable_if_t<std::is_convertible_v<String, std::basic_string_view<Char, Traits>>>>
    constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, String&& fmt, Args&&... args)
    {
        return internal::format<internal::output, Char, Traits>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename T>
    constexpr std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, T&& arg)
    {
        return internal::put<internal::output, Char, Traits>(stream, std::forward<T>(arg));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename String, typename = std::enable_if_t<std::is_convertible_v<String, std::basic_string_view<Char, Traits>>>>
    constexpr std::basic_ostream<Char, Traits>& vprint(std::basic_ostream<Char, Traits>& stream, String&& fmt, internal::arg_list_t<internal::stream_t<internal::output, Char, Traits>>&& args)
    {
        return internal::vformat<internal::output, Char, Traits>(stream, fmt, std::move(args));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename String, typename... Args, typename = std::enable_if_t<std::is_convertible_v<String, std::basic_string_view<Char, Traits>>>>
    constexpr std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream, String&& fmt, Args&&... args)
    {
        return internal::format<internal::output, Char, Traits>(stream, fmt, std::forward<Args>(args)...) << std::endl;
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename T>
    constexpr std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream, T&& arg)
    {
        return internal::put<internal::output, Char, Traits>(stream, std::forward<T>(arg)) << std::endl;
    }
    template <typename Char, typename Traits = std::char_traits<Char>>
    constexpr std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream)
    {
        return stream << std::endl;
    }

    //char IO
    template <typename... Args>
    constexpr std::istream& scan(std::string_view fmt, Args&&... args)
    {
        return scan(std::cin, fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    constexpr std::istream& scan(T&& arg)
    {
        return internal::put<internal::input, char, std::char_traits<char>>(std::cin, std::forward<T>(arg));
    }
    template <typename... Args>
    constexpr std::ostream& print(std::string_view fmt, Args&&... args)
    {
        return print(std::cout, fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    constexpr std::ostream& print(T&& arg)
    {
        return internal::put<internal::output, char, std::char_traits<char>>(std::cout, std::forward<T>(arg));
    }
    template <typename... Args>
    constexpr std::ostream& println(std::string_view fmt, Args&&... args)
    {
        return println(std::cout, fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    constexpr std::ostream& println(T&& arg)
    {
        return print(std::forward<T>(arg)) << std::endl;
    }
    constexpr decltype(auto) println()
    {
        return print(std::endl<char, std::char_traits<char>>);
    }

    //wchar_t IO
    template <typename... Args>
    constexpr std::wistream& wscan(std::wstring_view fmt, Args&&... args)
    {
        return scan(std::wcin, fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    constexpr std::wistream& wscan(T&& arg)
    {
        return internal::put<internal::input, wchar_t, std::char_traits<wchar_t>>(std::wcin, std::forward<T>(arg));
    }
    template <typename... Args>
    constexpr std::wostream& wprint(std::wstring_view fmt, Args&&... args)
    {
        return print(std::wcout, fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    constexpr std::wostream& wprint(T&& arg)
    {
        return internal::put<internal::output, wchar_t, std::char_traits<wchar_t>>(std::wcout, std::forward<T>(arg));
    }
    template <typename... Args>
    constexpr std::wostream& wprintln(std::wstring_view fmt, Args&&... args)
    {
        return println(std::wcout, fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    constexpr std::wostream& wprintln(T&& arg)
    {
        return wprint(std::forward<T>(arg)) << std::endl;
    }
    constexpr decltype(auto) wprintln()
    {
        return wprint(std::endl<wchar_t, std::char_traits<wchar_t>>);
    }
} // namespace sf

#endif // !SF_FORMAT_HPP
