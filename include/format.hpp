﻿#ifndef SF_FORMAT_HPP
#define SF_FORMAT_HPP

#if !defined(SF_CXX11) && __cplusplus >= 201103L
#define SF_CXX11
#endif // C++11

#if !defined(SF_CXX14) && __cplusplus >= 201402L
#define SF_CXX14
#endif // C++14

#if !defined(SF_CXX17) && __cplusplus >= 201703L
#define SF_CXX17
#endif // C++17

#ifndef SF_CONSTEXPR
#if defined(SF_CXX14) || _MSC_VER >= 1910
#define SF_CONSTEXPR constexpr
#else
#define SF_CONSTEXPR inline
#endif // SF_CXX14
#endif // !SF_CONSTEXPR

#ifndef SF_IF_CONSTEXPR
#if defined(SF_CXX17) || _MSC_VER >= 1911
#define SF_IF_CONSTEXPR if constexpr
#else
#define SF_IF_CONSTEXPR if
#endif // SF_CXX17
#endif // !SF_IF_CONSTEXPR

#ifndef SF_NOEXCEPT
#if defined(SF_CXX11) || _MSC_VER >= 1900
#define SF_NOEXCEPT noexcept
#else
#define SF_NOEXCEPT throw()
#endif // SF_CXX11
#endif // !SF_NOEXCEPT

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
        enum io_state
        {
            input,
            output
        };

        //Define types.
        template <io_state, typename Char, typename Traits>
        struct arg_stream;
        template <typename Char, typename Traits>
        struct arg_stream<input, Char, Traits>
        {
            typedef std::basic_istream<Char, Traits> type;
        };
        template <typename Char, typename Traits>
        struct arg_stream<output, Char, Traits>
        {
            typedef std::basic_ostream<Char, Traits> type;
        };

        template <io_state IOState, typename Char, typename Traits>
        using arg_stream_t = typename arg_stream<IOState, Char, Traits>::type;

        template <io_state IOState, typename Char, typename Traits>
        struct arg
        {
            typedef arg_stream_t<IOState, Char, Traits> stream_type;
            typedef std::function<stream_type&(stream_type&)> type;
            typedef std::vector<type> list_type;
        };

        template <io_state IOState, typename Char, typename Traits>
        using arg_t = typename arg<IOState, Char, Traits>::type;
        template <io_state IOState, typename Char, typename Traits>
        using arg_list_t = typename arg<IOState, Char, Traits>::list_type;

        template <typename T>
        struct forward_arg
        {
            typedef T&& type;
            typedef T var_type;
        };
        template <typename T>
        struct forward_arg<T*>
        {
            typedef T* type;
            typedef T* var_type;
        };

        template <typename T>
        using forward_arg_t = typename forward_arg<T>::type;
        template <typename T>
        using forward_arg_var_t = typename forward_arg<T>::var_type;

        //A packed arg.
        template <io_state IOState, typename T, typename Char, typename Traits>
        class arg_io
        {
        public:
            typedef forward_arg_t<T> f_arg_type;
            typedef forward_arg_var_t<T> f_arg_var_type;
            typedef arg_stream_t<IOState, Char, Traits> stream_type;

        private:
            f_arg_var_type arg;

        public:
            arg_io(f_arg_type arg) : arg(arg) {}
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

#define SF_CHAR_TEMPLATE(name, value)       \
    template <typename Char>                \
    SF_CONSTEXPR Char name() SF_NOEXCEPT;   \
    template <>                             \
    SF_CONSTEXPR char name() SF_NOEXCEPT    \
    {                                       \
        return value;                       \
    }                                       \
    template <>                             \
    SF_CONSTEXPR wchar_t name() SF_NOEXCEPT \
    {                                       \
        return L##value;                    \
    }
#define SF_STR_TEMPLATE(name, value)               \
    template <typename Char>                       \
    SF_CONSTEXPR const Char* name() SF_NOEXCEPT;   \
    template <>                                    \
    SF_CONSTEXPR const char* name() SF_NOEXCEPT    \
    {                                              \
        return value;                              \
    }                                              \
    template <>                                    \
    SF_CONSTEXPR const wchar_t* name() SF_NOEXCEPT \
    {                                              \
        return L##value;                           \
    }

        SF_CHAR_TEMPLATE(space, ' ')
        SF_CHAR_TEMPLATE(tab, '\t')
        SF_CHAR_TEMPLATE(cr, '\r')
        SF_CHAR_TEMPLATE(lf, '\n')

        SF_CHAR_TEMPLATE(left_brace, '{')
        SF_STR_TEMPLATE(left_double_brace, "{{")
        SF_CHAR_TEMPLATE(right_brace, '}')
        SF_STR_TEMPLATE(right_double_brace, "}}")

        //Input/Output string slice.
        template <io_state, typename Char, typename Traits>
        class string_view_io;
        template <typename Char, typename Traits>
        class string_view_io<input, Char, Traits>
        {
        public:
            typedef arg_stream_t<input, Char, Traits> stream_type;
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
            typedef arg_stream_t<output, Char, Traits> stream_type;
            typedef std::basic_string_view<Char, Traits> string_view_type;
            typedef typename string_view_type::size_type int_type;

        private:
            string_view_type arg;
            std::vector<string_view_type> split_by_double_brace()
            {
                std::vector<string_view_type> result;
                int_type index = 0, offset = 0;
                while ((index = arg.find(right_double_brace<Char>(), offset)) != string_view_type::npos)
                {
                    result.push_back(arg.substr(offset, index - offset + 1));
                    offset = index + 2;
                }
                if (arg.length() > offset)
                {
                    result.push_back(arg.substr(offset));
                }
                return result;
            }

        public:
            string_view_io(string_view_type&& arg) : arg(std::move(arg)) {}
            stream_type& operator()(stream_type& os)
            {
                if (os)
                {
                    for (auto& s : split_by_double_brace())
                    {
                        os.write(s.data(), s.length());
                    }
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
            typedef arg_t<IOState, Char, Traits> arg_type;
            typedef arg_stream_t<IOState, Char, Traits> stream_type;
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
                std::ios_base::fmtflags oldf;
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
            typedef arg_t<IOState, Char, Traits> arg_type;
            typedef arg_list_t<IOState, Char, Traits> arg_list_type;
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
                        index = fmt.find(left_brace<Char>(), offset);
                        int_type off = offset;
                        if (index != string_view_type::npos)
                        {
                            if (index + 1 < length && fmt[index + 1] == left_brace<Char>())
                            {
                                index++;
                            }
                            else
                            {
                                in_number = true;
                            }
                        }
                        else
                        {
                            index = length;
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
                            throw std::out_of_range("No \"}\" was found after \"{\".");
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
        SF_CONSTEXPR arg_stream_t<IOState, Char, Traits>& format(arg_stream_t<IOState, Char, Traits>& stream, format_string_view<IOState, Char, Traits> fmt)
        {
            arg_t<IOState, Char, Traits> arg;
            while (arg = fmt.move_next())
            {
                arg(stream);
            }
            return stream;
        }
        template <io_state IOState, typename Char, typename Traits, typename... Args>
        SF_CONSTEXPR arg_stream_t<IOState, Char, Traits>& format(arg_stream_t<IOState, Char, Traits>& stream, const std::basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            return format(stream, format_string_view<IOState, Char, Traits>(fmt, arg_list_t<IOState, Char, Traits>{ arg_io<IOState, Args, Char, Traits>(static_cast<forward_arg_t<Args>>(args))... }));
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
