// StreamFormat format.hpp
#ifndef SF_FORMAT_HPP
#define SF_FORMAT_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX17) || _MSC_VER >= 1910

#include <functional>
#include <iostream>
#include <string>
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

        //A packed arg.
        template <io_state IOState, typename T, typename Char, typename Traits>
        class arg_io
        {
        public:
            typedef stream_t<IOState, Char, Traits> stream_type;

        private:
            T arg;

        public:
            arg_io(T&& arg) : arg(arg) {}
            stream_type& operator()(stream_type& stream)
            {
                SF_IF_CONSTEXPR(IOState == input)
                {
                    return stream >> arg;
                }
                else SF_IF_CONSTEXPR(IOState == output)
                {
                    return stream << arg;
                }
                else
                {
                    return stream;
                }
            }
        };

        SF_CHAR_TEMPLATE(space, ' ')
        SF_CHAR_TEMPLATE(tab, '\t')
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
            typedef std::basic_string_view<Char, Traits> string_view_type;

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
                                    (Traits::eq(t, space<Char>()) || Traits::eq(t, tab<Char>()) || Traits::eq(t, cr<Char>()) || Traits::eq(t, lf<Char>())))
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
            typedef std::basic_string_view<Char, Traits> string_view_type;
            typedef typename string_view_type::size_type int_type;

        private:
            string_view_type arg;

        public:
            string_view_io(string_view_type&& arg) : arg(std::move(arg)) {}
            stream_type& operator()(stream_type& os)
            {
                if (os)
                {
                    os.write(arg.data(), arg.length());
                }
                return os;
            }
        };

        SF_CHAR_TEMPLATE(colon, ':')

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
        class format_arg_io
        {
        public:
            typedef stream_t<IOState, Char, Traits> stream_type;
            typedef arg_t<stream_type> arg_type;
            typedef std::basic_string_view<Char, Traits> string_view_type;

        private:
            arg_type& ori;
            Char fmtc;
            int fmtf;

        public:
            format_arg_io(arg_type& ori, const string_view_type& str) : ori(ori), fmtf(0)
            {
                if (str.empty())
                {
                    fmtc = cgen<Char>();
                }
                else
                {
                    fmtc = str[0];
                    if (str.length() > 1)
                    {
                        fmtf = std::stoi(str.substr(1).data());
                    }
                }
            }
            stream_type& operator()(stream_type& stream)
            {
                std::ios_base::fmtflags oldf = (std::ios_base::fmtflags)0;
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
                else if (Traits::eq(fmtc, cgen<Char>()) || Traits::eq(fmtc, cGEN<Char>()))
                {
                }
                else
                {
                    throw std::logic_error("Invalid format character.");
                }
                if (Traits::eq(fmtc, cDEC<Char>()) || Traits::eq(fmtc, cOCT<Char>()) || Traits::eq(fmtc, cHEX<Char>()) || Traits::eq(fmtc, cSCI<Char>()) || Traits::eq(fmtc, cFIX<Char>()) || Traits::eq(fmtc, cGEN<Char>()) || Traits::eq(fmtc, cLFT<Char>()) || Traits::eq(fmtc, cRIT<Char>()) || Traits::eq(fmtc, cITN<Char>()))
                {
                    stream.setf(std::ios_base::uppercase);
                }
                else
                {
                    stream.unsetf(std::ios_base::uppercase);
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
            typedef std::basic_string_view<Char, Traits> string_view_type;
            typedef typename string_view_type::size_type int_type;
            typedef string_view_io<IOState, Char, Traits> string_view_io_type;

        private:
            const string_view_type& fmt;
            arg_list_type args;
            int_type offset, index;
            const int_type length;
            bool in_number;

        public:
            SF_CONSTEXPR format_string_view(const string_view_type& fmt, arg_list_type&& args)
                : fmt(fmt), args(std::move(args)), offset(0), index(0), length(fmt.length()), in_number(false)
            {
            }
            arg_type move_next()
            {
                while (offset < length)
                {
                    if (!in_number)
                    {
                        int_type off = offset;
                        int_type lindex = fmt.find(left_brace<Char>(), offset);
                        int_type rindex = fmt.find(right_brace<Char>(), offset);
                        if (lindex == string_view_type::npos && rindex == string_view_type::npos)
                        {
                            index = length;
                        }
                        else
                        {
                            if (lindex < rindex)
                            {
                                if (lindex + 1 < length && Traits::eq(fmt[lindex + 1], left_brace<Char>()))
                                {
                                    index = lindex + 1;
                                }
                                else
                                {
                                    index = lindex;
                                    in_number = true;
                                }
                            }
                            else
                            {
                                if (rindex + 1 < length && Traits::eq(fmt[rindex + 1], right_brace<Char>()))
                                {
                                    index = rindex + 1;
                                }
                                else
                                {
                                    throw std::logic_error("No \"{\" matches \"}\".");
                                }
                            }
                        }
                        int_type len = index - offset;
                        offset = index + 1;
                        if (len <= 0)
                            continue;
                        return string_view_io_type(fmt.substr(off, len));
                    }
                    else
                    {
                        index = fmt.find(right_brace<Char>(), offset);
                        if (index == string_view_type::npos)
                        {
                            throw std::logic_error("No \"}\" was found after \"{\".");
                        }
                        else
                        {
                            in_number = false;
                            int_type ci = fmt.find(colon<Char>(), offset);
                            if (ci != string_view_type::npos)
                            {
                                std::size_t i = std::stoull(fmt.substr(offset, ci - offset).data());
                                offset = index + 1;
                                return format_arg_io<IOState, Char, Traits>(args[i], fmt.substr(ci + 1, index - ci - 1));
                            }
                            else
                            {
                                std::size_t i = std::stoull(fmt.substr(offset, index - offset).data());
                                offset = index + 1;
                                return args[i];
                            }
                        }
                    }
                    break;
                }
                return {};
            }
        };

        //Iterates the format string and arguments.
        template <io_state IOState, typename Char, typename Traits>
        SF_CONSTEXPR stream_t<IOState, Char, Traits>& format(stream_t<IOState, Char, Traits>& stream, format_string_view<IOState, Char, Traits> fmt)
        {
            arg_t<stream_t<IOState, Char, Traits>> arg;
            while ((arg = fmt.move_next()))
            {
                arg(stream);
            }
            return stream;
        }
        template <io_state IOState, typename Char, typename Traits, typename... Args>
        SF_CONSTEXPR stream_t<IOState, Char, Traits>& format(stream_t<IOState, Char, Traits>& stream, const std::basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            return format(stream, format_string_view<IOState, Char, Traits>(fmt, arg_list_t<stream_t<IOState, Char, Traits>>{ arg_io<IOState, Args, Char, Traits>(static_cast<Args&&>(args))... }));
        }
    } // namespace internal

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    SF_CONSTEXPR std::basic_istream<Char, Traits>& scan(std::basic_istream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::format<internal::input>(stream, fmt, args...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename... Args>
    SF_CONSTEXPR std::basic_ostream<Char, Traits>& print(std::basic_ostream<Char, Traits>& stream, std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::format<internal::output>(stream, fmt, args...);
    }

    //char IO
    template <typename... Args>
    SF_CONSTEXPR std::istream& scan(std::string_view fmt, Args&&... args)
    {
        return scan(std::cin, fmt, args...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::ostream& print(std::string_view fmt, Args&&... args)
    {
        return print(std::cout, fmt, args...);
    }

    //wchar_t IO
    template <typename... Args>
    SF_CONSTEXPR std::wistream& scan(std::wstring_view fmt, Args&&... args)
    {
        return scan(std::wcin, fmt, args...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::wostream& print(std::wstring_view fmt, Args&&... args)
    {
        return print(std::wcout, fmt, args...);
    }
} // namespace sf
#endif // SF_CXX17

#endif // !SF_FORMAT_HPP
