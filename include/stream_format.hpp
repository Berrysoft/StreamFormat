#ifndef SF_H
#define SF_H

#if __cplusplus >= 201103L
#define SF_CXX11
#endif // C++11

#if __cplusplus >= 201402L
#define SF_CXX14
#endif // C++14

#if __cplusplus >= 201703L
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

#if defined(SF_CXX17) || _MSC_VER >= 1900

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace stream_format
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

        template <io_state IOState, typename Char, typename Traits = std::char_traits<Char>>
        using arg_stream_t = typename arg_stream<IOState, Char, Traits>::type;

        template <io_state IOState, typename Char, typename Traits>
        struct arg
        {
            typedef arg_stream_t<IOState, Char, Traits> stream_type;
            typedef std::function<stream_type&(stream_type&)> type;
            typedef std::vector<type> list_type;
        };

        template <io_state IOState, typename Char, typename Traits = std::char_traits<Char>>
        using arg_t = typename arg<IOState, Char, Traits>::type;
        template <io_state IOState, typename Char, typename Traits = std::char_traits<Char>>
        using arg_list_t = typename arg<IOState, Char, Traits>::list_type;

        template <io_state, typename T>
        struct forward_arg;
        template <typename T>
        struct forward_arg<input, T>
        {
            typedef T& type;
            typedef T& var_type;
        };
        template <typename T>
        struct forward_arg<output, T>
        {
            typedef T&& type;
            typedef T var_type;
        };
        template <io_state IOState, typename T>
        struct forward_arg<IOState, T*>
        {
            typedef T* type;
            typedef T* var_type;
        };

        template <io_state IOState, typename T>
        using forward_arg_t = typename forward_arg<IOState, T>::type;
        template <io_state IOState, typename T>
        using forward_arg_var_t = typename forward_arg<IOState, T>::var_type;

        template <io_state IOState, typename T, typename Char, typename Traits = std::char_traits<Char>>
        class arg_io
        {
        public:
            typedef forward_arg_t<IOState, T> f_arg_type;
            typedef forward_arg_var_t<IOState, T> f_arg_var_type;
            typedef arg_stream_t<IOState, Char, Traits> stream_type;

        private:
            f_arg_var_type m_arg;

        public:
            arg_io(f_arg_type arg) : m_arg(arg) {}
            stream_type& operator()(stream_type& stream)
            {
                SF_IF_CONSTEXPR(IOState == input)
                {
                    return stream >> m_arg;
                }
                else SF_IF_CONSTEXPR(IOState == output)
                {
                    return stream << m_arg;
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
        template <io_state, typename Char, typename Traits = std::char_traits<Char>>
        class string_view_io;
        template <typename Char, typename Traits>
        class string_view_io<input, Char, Traits>
        {
        public:
            typedef Traits traits_type;
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
                        if (traits_type::eq(c, space<Char>()))
                        {
                            while (true)
                            {
                                auto t = is.peek();
                                if (t != traits_type::eof() &&
                                    (traits_type::eq(t, space<Char>()) || traits_type::eq(t, tab<Char>()) || traits_type::eq(t, cr<Char>()) || traits_type::eq(t, lf<Char>())))
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
                                if (t == traits_type::eof() || traits_type::eq(t, c))
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

        private:
            string_view_type arg;

        public:
            string_view_io(string_view_type&& arg) : arg(std::move(arg)) {}
            stream_type& operator()(stream_type& os)
            {
                if (os)
                {
                    return os.write(&arg.front(), arg.length());
                }
                return os;
            }
        };

        //A flag indicates the type of current clice.
        enum format_string_token
        {
            text,
            number,
            brace
        };
        //A pack of format string and arguments.
        template <io_state IOState, typename Char, typename Traits = std::char_traits<Char>>
        class format_string_view
        {
        public:
            typedef arg_t<IOState, Char, Traits> arg_type;
            typedef arg_list_t<IOState, Char, Traits> arg_list_type;
            typedef std::basic_string_view<Char, Traits> string_view_type;
            typedef string_view_io<IOState, Char, Traits> string_view_io_type;

        private:
            const string_view_type& fmt;
            arg_list_type args;
            std::size_t offset, index;
            const std::size_t length;
            format_string_token state;

        public:
            SF_CONSTEXPR format_string_view(const string_view_type& fmt, arg_list_type&& args)
                : fmt(fmt), args(std::move(args)), offset(0), index(0), length(fmt.length()), state(text)
            {
            }
            arg_type move_next()
            {
                while (offset < length)
                {
                    switch (state)
                    {
                    case text:
                        index = fmt.find(left_brace<Char>(), offset);
                        if (index != string_view_type::npos)
                        {
                            if (index + 1 < length)
                            {
                                if (fmt[index + 1] == left_brace<Char>())
                                {
                                    state = brace;
                                    std::size_t off = offset;
                                    std::size_t len = index - offset + 1;
                                    offset = index + 2;
                                    if (len <= 0)
                                        continue;
                                    return string_view_io_type(fmt.substr(off, len));
                                }
                            }
                            state = number;
                            std::size_t off = offset;
                            std::size_t len = index - offset;
                            offset = index + 1;
                            if (len <= 0)
                                continue;
                            return string_view_io_type(fmt.substr(off, len));
                        }
                        else
                        {
                            std::size_t off = offset;
                            std::size_t len = length - offset;
                            offset = length;
                            if (len <= 0)
                                continue;
                            return string_view_io_type(fmt.substr(off, len));
                        }
                        break;
                    case number:
                        index = fmt.find(right_brace<Char>(), offset);
                        if (index == string_view_type::npos)
                        {
                            throw std::out_of_range("No \"}\" found after \"{\".");
                        }
                        else
                        {
                            state = text;
                            std::size_t i = std::stoull(fmt.substr(offset, index - offset).data());
                            offset = index + 1;
                            return args[i];
                        }
                        break;
                    case brace:
                        index = fmt.find(right_double_brace<Char>(), offset);
                        if (index == string_view_type::npos)
                        {
                            throw std::out_of_range("No \"}}\" found after \"{{\".");
                        }
                        else
                        {
                            state = text;
                            std::size_t off = offset;
                            std::size_t len = index - offset + 1;
                            offset = index + 2;
                            if (len <= 0)
                                continue;
                            return string_view_io_type(fmt.substr(off, len));
                        }
                        break;
                    }
                    break;
                }
                return {};
            }
        };

        //Iterates the format string and arguments.
        template <io_state IOState, typename Char, typename Traits = std::char_traits<Char>>
        SF_CONSTEXPR arg_stream_t<IOState, Char, Traits>& format(arg_stream_t<IOState, Char, Traits>& stream, format_string_view<IOState, Char, Traits> fmt)
        {
            arg_t<IOState, Char, Traits> arg;
            while (arg = fmt.move_next())
            {
                arg(stream);
            }
            return stream;
        }
        template <typename Char, typename... Args, typename Traits = std::char_traits<Char>>
        SF_CONSTEXPR arg_stream_t<input, Char, Traits>& scan(arg_stream_t<input, Char, Traits>& stream, const std::basic_string_view<Char, Traits>& fmt, Args&... args)
        {
            return format(stream, format_string_view<input, Char, Traits>(fmt, arg_list_t<input, Char>{ arg_io<input, Args, Char, Traits>(static_cast<forward_arg_t<input, Args>>(args))... }));
        }
        template <typename Char, typename... Args, typename Traits = std::char_traits<Char>>
        SF_CONSTEXPR arg_stream_t<output, Char, Traits>& print(arg_stream_t<output, Char, Traits>& stream, const std::basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            return format(stream, format_string_view<output, Char, Traits>(fmt, arg_list_t<output, Char, Traits>{ arg_io<output, Args, Char, Traits>(static_cast<forward_arg_t<output, Args>>(args))... }));
        }
    } // namespace internal

    template <typename... Args>
    SF_CONSTEXPR std::ostream& print(std::ostream& os, std::string_view fmt, Args&&... args)
    {
        return internal::print(os, fmt, args...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::ostream& print(std::string_view fmt, Args&&... args)
    {
        return print(std::cout, fmt, args...);
    }

    template <typename... Args>
    SF_CONSTEXPR std::istream& scan(std::istream& is, std::string_view fmt, Args&... args)
    {
        return internal::scan(is, fmt, args...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::istream& scan(std::string_view fmt, Args&... args)
    {
        return scan(std::cin, fmt, args...);
    }

    template <typename... Args>
    SF_CONSTEXPR std::wostream& print(std::wostream& os, std::wstring_view fmt, Args&&... args)
    {
        return internal::print(os, fmt, args...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::wostream& print(std::wstring_view fmt, Args&&... args)
    {
        return print(std::wcout, fmt, args...);
    }

    template <typename... Args>
    SF_CONSTEXPR std::wistream& scan(std::wistream& is, std::wstring_view fmt, Args&... args)
    {
        return internal::scan(is, fmt, args...);
    }
    template <typename... Args>
    SF_CONSTEXPR std::wistream& scan(std::wstring_view fmt, Args&... args)
    {
        return scan(std::wcin, fmt, args...);
    }
} // namespace stream_format
#endif // SF_CXX17

#endif // SF_H
