/**StreamFormat format.hpp
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
#ifndef SF_FORMAT_HPP
#define SF_FORMAT_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX11)

#include <functional>
#include <iostream>
#include <sf/stoi.hpp>
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
            typedef std::basic_istream<Char, Traits> type;
        };
        template <typename Char, typename Traits>
        struct stream<output, Char, Traits>
        {
            typedef std::basic_ostream<Char, Traits> type;
        };

        template <io_state IOState, typename Char, typename Traits>
        using stream_t = typename stream<IOState, Char, Traits>::type;

        template <typename Stream>
        struct arg
        {
            typedef std::function<Stream&(Stream&)> type;
            typedef std::vector<type> list_type;
        };

        template <typename Stream>
        using arg_t = typename arg<Stream>::type;
        template <typename Stream>
        using arg_list_t = typename arg<Stream>::list_type;

        template <io_state IOState, typename T, typename Stream>
        SF_CONSTEXPR typename std::enable_if<IOState == input, Stream>::type& operate_arg_io(Stream& stream, T&& arg)
        {
            return stream >> arg;
        }
        template <io_state IOState, typename T, typename Stream>
        SF_CONSTEXPR typename std::enable_if<IOState == output, Stream>::type& operate_arg_io(Stream& stream, T&& arg)
        {
            return stream << arg;
        }

        //A packed arg.
        template <io_state IOState, typename T, typename Char, typename Traits>
        class arg_io
        {
        public:
            typedef stream_t<IOState, Char, Traits> stream_type;

        private:
            T arg;

        public:
            arg_io(T&& arg) : arg(std::forward<T>(arg)) {}
            SF_CONSTEXPR stream_type& operator()(stream_type& stream)
            {
                //G++ will error with static_cast and -std=c++11.
                return operate_arg_io<IOState, T, stream_type>(stream, (T &&) arg);
            }
        };

        SF_CHAR_TEMPLATE(space, ' ')
        SF_CHAR_TEMPLATE(tab, '\t')
        SF_CHAR_TEMPLATE(vtab, '\v')
        SF_CHAR_TEMPLATE(cr, '\r')
        SF_CHAR_TEMPLATE(lf, '\n')

        SF_CHAR_TEMPLATE(left_brace, '{')
        SF_CHAR_TEMPLATE(right_brace, '}')

        //Input/Output string slice.
        template <io_state, typename Char, typename Traits>
        class string_view_io;
        template <typename Char, typename Traits>
        class string_view_io<input, Char, Traits>
        {
        public:
            typedef stream_t<input, Char, Traits> stream_type;
            typedef basic_string_view<Char, Traits> string_view_type;

        private:
            string_view_type arg;

        public:
            string_view_io(string_view_type&& arg) : arg(std::move(arg)) {}
            stream_type& operator()(stream_type& is)
            {
                if (is)
                {
                    for (const Char& c : arg)
                    {
                        if (Traits::eq(c, space<Char>()))
                        {
                            while (true)
                            {
                                auto t = is.peek();
                                if (t != Traits::eof() &&
                                    (Traits::eq(t, space<Char>()) || Traits::eq(t, tab<Char>()) || Traits::eq(t, vtab<Char>()) || Traits::eq(t, cr<Char>()) || Traits::eq(t, lf<Char>())))
                                {
                                    is.get();
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
                                auto t = is.get();
                                if (t == Traits::eof() || Traits::eq(t, c))
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                return is;
            }
        };
        template <typename Char, typename Traits>
        class string_view_io<output, Char, Traits>
        {
        public:
            typedef stream_t<output, Char, Traits> stream_type;
            typedef basic_string_view<Char, Traits> string_view_type;
            typedef typename string_view_type::size_type int_type;

        private:
            string_view_type arg;

        public:
            string_view_io(string_view_type&& arg) : arg(std::move(arg)) {}
            stream_type& operator()(stream_type& os)
            {
                if (!arg.empty())
                {
                    os.write(arg.data(), arg.length());
                }
                return os;
            }
        };

        SF_CHAR_TEMPLATE(colon, ':')
        SF_CHAR_TEMPLATE(comma, ',')

        SF_CHAR_TEMPLATE(cbla, 'b')
        SF_CHAR_TEMPLATE(cBLA, 'B')

        SF_CHAR_TEMPLATE(cdec, 'd')
        SF_CHAR_TEMPLATE(cDEC, 'D')

        SF_CHAR_TEMPLATE(csci, 'e')
        SF_CHAR_TEMPLATE(cSCI, 'E')

        SF_CHAR_TEMPLATE(cfix, 'f')
        SF_CHAR_TEMPLATE(cFIX, 'F')

        SF_CHAR_TEMPLATE(cgen, 'g')
        SF_CHAR_TEMPLATE(cGEN, 'G')

        SF_CHAR_TEMPLATE(citn, 'i')
        SF_CHAR_TEMPLATE(cITN, 'I')

        SF_CHAR_TEMPLATE(clft, 'l')
        SF_CHAR_TEMPLATE(cLFT, 'L')

        SF_CHAR_TEMPLATE(coct, 'o')
        SF_CHAR_TEMPLATE(cOCT, 'O')

        SF_CHAR_TEMPLATE(crit, 'r')
        SF_CHAR_TEMPLATE(cRIT, 'R')

        SF_CHAR_TEMPLATE(chex, 'x')
        SF_CHAR_TEMPLATE(cHEX, 'X')

        //Parse format string and set stream flag.
        //B -> boolalpha
        //D -> dec
        //E -> scientific
        //F -> fixed
        //G -> <nop>/uppercase
        //I -> internal
        //L -> left
        //O -> oct
        //R -> right
        //X -> hex
        template <io_state IOState, typename Char, typename Traits>
        class format_opt
        {
        public:
            typedef stream_t<IOState, Char, Traits> stream_type;
            typedef basic_string_view<Char, Traits> string_view_type;

        private:
            Char fmtc;
            int fmtf;

        public:
            format_opt(Char fmtc, int fmtf) : fmtc(fmtc), fmtf(fmtf) {}
            std::ios_base::fmtflags setf(stream_type& stream)
            {
                std::ios_base::fmtflags oldf = static_cast<std::ios_base::fmtflags>(0);
                //Determine chars.
                if (Traits::eq(fmtc, cdec<Char>()) || Traits::eq(fmtc, cDEC<Char>()))
                {
                    oldf = stream.setf(std::ios_base::dec, std::ios_base::basefield);
                    stream.fill('0');
                    stream.width(fmtf);
                }
                else if (Traits::eq(fmtc, coct<Char>()) || Traits::eq(fmtc, cOCT<Char>()))
                {
                    oldf = stream.setf(std::ios_base::oct, std::ios_base::basefield);
                    stream.fill('0');
                    stream.width(fmtf);
                }
                else if (Traits::eq(fmtc, chex<Char>()) || Traits::eq(fmtc, cHEX<Char>()))
                {
                    oldf = stream.setf(std::ios_base::hex, std::ios_base::basefield);
                    stream.fill('0');
                    stream.width(fmtf);
                }
                else if (Traits::eq(fmtc, csci<Char>()) || Traits::eq(fmtc, cSCI<Char>()))
                {
                    oldf = stream.setf(std::ios_base::scientific, std::ios_base::floatfield);
                    stream.precision(fmtf);
                }
                else if (Traits::eq(fmtc, cfix<Char>()) || Traits::eq(fmtc, cFIX<Char>()))
                {
                    oldf = stream.setf(std::ios_base::fixed, std::ios_base::floatfield);
                    stream.precision(fmtf);
                }
                else if (Traits::eq(fmtc, clft<Char>()) || Traits::eq(fmtc, cLFT<Char>()))
                {
                    oldf = stream.setf(std::ios_base::left, std::ios_base::adjustfield);
                    stream.fill(' ');
                    stream.width(fmtf);
                }
                else if (Traits::eq(fmtc, crit<Char>()) || Traits::eq(fmtc, cRIT<Char>()))
                {
                    oldf = stream.setf(std::ios_base::right, std::ios_base::adjustfield);
                    stream.fill(' ');
                    stream.width(fmtf);
                }
                else if (Traits::eq(fmtc, citn<Char>()) || Traits::eq(fmtc, cITN<Char>()))
                {
                    oldf = stream.setf(std::ios_base::internal, std::ios_base::adjustfield);
                    stream.fill(' ');
                    stream.width(fmtf);
                }
                else if (Traits::eq(fmtc, cbla<Char>()) || Traits::eq(fmtc, cBLA<Char>()))
                {
                    oldf = stream.setf(std::ios_base::boolalpha);
                }
                else if (Traits::eq(fmtc, cgen<Char>()) || Traits::eq(fmtc, cGEN<Char>()))
                {
                    //General, doesn't need any flags.
                }
                else
                {
                    throw std::logic_error("Invalid format character.");
                }
                //Uppercase.
                if (Traits::eq(fmtc, cDEC<Char>()) || Traits::eq(fmtc, cOCT<Char>()) || Traits::eq(fmtc, cHEX<Char>()) || Traits::eq(fmtc, cSCI<Char>()) || Traits::eq(fmtc, cFIX<Char>()) || Traits::eq(fmtc, cGEN<Char>()) || Traits::eq(fmtc, cLFT<Char>()) || Traits::eq(fmtc, cRIT<Char>()) || Traits::eq(fmtc, cITN<Char>()) || Traits::eq(fmtc, cBLA<Char>()))
                {
                    stream.setf(std::ios_base::uppercase);
                }
                else
                {
                    stream.unsetf(std::ios_base::uppercase);
                }
                return oldf;
            }
        };

        template <io_state IOState, typename Char, typename Traits>
        class format_arg_io
        {
        public:
            typedef stream_t<IOState, Char, Traits> stream_type;
            typedef arg_t<stream_type> arg_type;
            typedef basic_string_view<Char, Traits> string_view_type;
            typedef typename string_view_type::size_type int_type;

        private:
            arg_type& ori;
            const string_view_type& fmts;

        public:
            format_arg_io(arg_type& ori, const string_view_type& fmts) : ori(ori), fmts(fmts) {}
            stream_type& operator()(stream_type& stream)
            {
                std::ios_base::fmtflags oldf = static_cast<std::ios_base::fmtflags>(0);
                int_type length = fmts.length();
                int_type offset = 0, index = 0;
                for (; index <= length; index++)
                {
                    if (index == length || Traits::eq(fmts[index], comma<Char>()))
                    {
                        Char fmtc = fmts[offset];
                        int fmtf = 0;
                        int_type len = index - offset - 1;
                        if (len > 0)
                        {
                            fmtf = stoi(fmts.substr(offset + 1, len));
                        }
                        oldf |= format_opt<IOState, Char, Traits>(fmtc, fmtf).setf(stream);
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
            typedef stream_t<IOState, Char, Traits> stream_type;
            typedef arg_t<stream_type> arg_type;
            typedef arg_list_t<stream_type> arg_list_type;
            typedef basic_string_view<Char, Traits> string_view_type;
            typedef typename string_view_type::size_type int_type;
            typedef string_view_io<IOState, Char, Traits> string_view_io_type;

        private:
            const string_view_type& fmt;
            arg_list_type args;

        public:
            SF_CONSTEXPR format_string_view(const string_view_type& fmt, arg_list_type&& args)
                : fmt(fmt), args(std::move(args))
            {
            }
            stream_type& operator()(stream_type& stream)
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
                            if (Traits::eq(fmt[index], left_brace<Char>()))
                            {
                                index++;
                                if (!(index < length && Traits::eq(fmt[index], left_brace<Char>())))
                                {
                                    in_number = true;
                                    index--;
                                }
                            }
                            else if (Traits::eq(fmt[index], right_brace<Char>()))
                            {
                                index++;
                                if (!(index < length && Traits::eq(fmt[index], right_brace<Char>())))
                                {
                                    throw std::logic_error("No \"{\" matches \"}\".");
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
                        string_view_io_type(fmt.substr(off, len))(stream);
                    }
                    else
                    {
                        for (index = offset; index < length; index++)
                        {
                            if (Traits::eq(fmt[index], right_brace<Char>()))
                                break;
                        }
                        if (index == length)
                        {
                            throw std::logic_error("No \"}\" was found after \"{\".");
                        }
                        else
                        {
                            in_number = false;
                            int_type ci;
                            for (ci = offset; ci < index; ci++)
                            {
                                if (Traits::eq(fmt[ci], colon<Char>()))
                                {
                                    break;
                                }
                            }
                            if (ci > offset)
                            {
                                arg_index = stoull(fmt.substr(offset, ci - offset));
                            }
                            if (arg_index >= args.size())
                            {
                                string_view_io_type(fmt.substr(offset - 1, index - offset + 2))(stream);
                            }
                            else if (index == ci)
                            {
                                args[arg_index](stream);
                            }
                            else
                            {
                                format_arg_io<IOState, Char, Traits>(args[arg_index], fmt.substr(ci + 1, index - ci - 1))(stream);
                            }
                            offset = index + 1;
                            arg_index++;
                        }
                    }
                }
                return stream;
            }
        };

        template <io_state IOState, typename Char, typename Traits, typename... Args>
        SF_CONSTEXPR stream_t<IOState, Char, Traits>& format(stream_t<IOState, Char, Traits>& stream, const basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            return format_string_view<IOState, Char, Traits>(fmt, arg_list_t<stream_t<IOState, Char, Traits>>{ arg_io<IOState, Args, Char, Traits>(std::forward<Args>(args))... })(stream);
        }

        template <io_state IOState, typename Char, typename Traits, typename T>
        SF_CONSTEXPR stream_t<IOState, Char, Traits>& format(stream_t<IOState, Char, Traits>& stream, T&& arg)
        {
            return arg_io<IOState, T, Char, Traits>(std::forward<T>(arg))(stream);
        }
    } // namespace internal

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    SF_CONSTEXPR std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::format<internal::input>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    SF_CONSTEXPR std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::format<internal::output>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    SF_CONSTEXPR std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream, basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::format<internal::output>(stream, fmt, std::forward<Args>(args)...) << std::endl;
    }

    //char IO
    template <typename... Args>
    SF_CONSTEXPR std::istream& scan(std::istream& stream, string_view fmt, Args&&... args)
    {
        return internal::format<internal::input>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::istream& scan(string_view fmt, Args&&... args)
    {
        return scan(std::cin, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::ostream& print(std::ostream& stream, string_view fmt, Args&&... args)
    {
        return internal::format<internal::output>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::ostream& print(string_view fmt, Args&&... args)
    {
        return print(std::cout, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::ostream& println(std::ostream& stream, string_view fmt, Args&&... args)
    {
        return internal::format<internal::output>(stream, fmt, std::forward<Args>(args)...) << std::endl;
    }
    template <typename... Args>
    SF_CONSTEXPR std::ostream& println(string_view fmt, Args&&... args)
    {
        return println(std::cout, fmt, std::forward<Args>(args)...);
    }

    //wchar_t IO
    template <typename... Args>
    SF_CONSTEXPR std::wistream& scan(std::wistream& stream, wstring_view fmt, Args&&... args)
    {
        return internal::format<internal::input>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::wistream& scan(wstring_view fmt, Args&&... args)
    {
        return scan(std::wcin, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::wostream& print(std::wostream& stream, wstring_view fmt, Args&&... args)
    {
        return internal::format<internal::output>(stream, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::wostream& print(wstring_view fmt, Args&&... args)
    {
        return print(std::wcout, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::wostream& println(std::wostream& stream, wstring_view fmt, Args&&... args)
    {
        return internal::format<internal::output>(stream, fmt, std::forward<Args>(args)...) << std::endl;
    }
    template <typename... Args>
    SF_CONSTEXPR std::wostream& println(wstring_view fmt, Args&&... args)
    {
        return println(std::wcout, fmt, std::forward<Args>(args)...);
    }

    //Simple IO functions for convinence.

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename T>
    SF_CONSTEXPR std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, T&& arg)
    {
        return internal::format<internal::input, Char, Traits, T>(stream, std::forward<T>(arg));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename T>
    SF_CONSTEXPR std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, T&& arg)
    {
        return internal::format<internal::output, Char, Traits, T>(stream, std::forward<T>(arg));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename T>
    SF_CONSTEXPR std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream, T&& arg)
    {
        return print<Char, Traits, T>(stream, std::forward<T>(arg)) << std::endl;
    }
    template <typename Char, typename Traits = std::char_traits<Char>>
    SF_CONSTEXPR std::basic_ostream<Char, Traits>& println(std::basic_ostream<Char, Traits>& stream)
    {
        return print<Char, Traits>(stream, std::endl<Char, Traits>);
    }
#ifndef SF_FORCE_WIDE_IO
    //char IO
    template <typename T>
    SF_CONSTEXPR std::istream& scan(T&& arg)
    {
        return scan(std::cin, std::forward<T>(arg));
    }
    template <typename T>
    SF_CONSTEXPR std::ostream& print(T&& arg)
    {
        return print(std::cout, std::forward<T>(arg));
    }
    template <typename T>
    SF_CONSTEXPR std::ostream& println(T&& arg)
    {
        return println(std::cout, std::forward<T>(arg));
    }
    SF_CONSTEXPR std::ostream& println()
    {
        return println<char>(std::cout);
    }
#else
    //wchar_t IO
    template <typename T>
    SF_CONSTEXPR std::wistream& scan(T&& arg)
    {
        return scan(std::wcin, std::forward<T>(arg));
    }
    template <typename T>
    SF_CONSTEXPR std::wostream& print(T&& arg)
    {
        return print(std::wcout, std::forward<T>(arg));
    }
    template <typename T>
    SF_CONSTEXPR std::wostream& println(T&& arg)
    {
        return println(std::wcout, std::forward<T>(arg));
    }
    SF_CONSTEXPR std::wostream& println()
    {
        return println<wchar_t>(std::wcout);
    }
#endif // !SF_FORCE_WIDE_IO
} // namespace sf
#endif // SF_CXX11

#endif // !SF_FORMAT_HPP
