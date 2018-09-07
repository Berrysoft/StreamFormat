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
#ifdef SF_CXX14
#define SF_CONSTEXPR constexpr
#else
#define SF_CONSTEXPR inline
#endif // SF_CXX14
#endif // !SF_CONSTEXPR

#ifndef SF_IF_CONSTEXPR
#ifdef SF_CXX17
#define SF_IF_CONSTEXPR constexpr
#else
#define SF_IF_CONSTEXPR
#endif // SF_CXX17
#endif // !SF_IF_CONSTEXPR

#ifndef SF_NOEXCEPT
#ifdef SF_CXX11
#define SF_NOEXCEPT noexcept
#else
#define SF_NOEXCEPT throw()
#endif // SF_CXX11
#endif // !SF_NOEXCEPT

#ifdef SF_CXX17

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
template <typename Char, io_state>
struct arg;
template <typename Char>
struct arg<Char, output>
{
    typedef std::basic_ostream<Char> stream_type;
    typedef std::function<stream_type&(stream_type&)> type;
    typedef std::vector<type> list_type;
};
template <typename Char>
struct arg<Char, input>
{
    typedef std::basic_istream<Char> stream_type;
    typedef std::function<stream_type&(stream_type&)> type;
    typedef std::vector<type> list_type;
};

//The type of an argument or a slice of string,
//which could be invoked.
template <typename Char, io_state IOState>
using arg_t = typename arg<Char, IOState>::type;
//An std::vector contains arg_t.
template <typename Char, io_state IOState>
using arg_list_t = typename arg<Char, IOState>::list_type;
//The type of stream for specified Char type.
template <typename Char, io_state IOState>
using arg_stream_t = typename arg<Char, IOState>::stream_type;

template <typename T, io_state>
struct forward_arg;
template <typename T>
struct forward_arg<T, output>
{
    typedef T&& type;
    typedef T var_type;
};
template <typename T>
struct forward_arg<T, input>
{
    typedef T& type;
    typedef T& var_type;
};
template <typename T, io_state IOState>
struct forward_arg<T*, IOState>
{
    typedef T* type;
    typedef T* var_type;
};

template <typename T, io_state IOState>
using forward_arg_t = typename forward_arg<T, IOState>::type;
template <typename T, io_state IOState>
using forward_arg_var_t = typename forward_arg<T, IOState>::var_type;
template <typename T>
using forward_output_arg_t = forward_arg_t<T, output>;
template <typename T>
using forward_input_arg_t = forward_arg_t<T, input>;

template <typename Char, typename T, io_state IOState>
class arg_io
{
public:
    typedef forward_arg_t<T, IOState> f_arg_type;
    typedef forward_arg_var_t<T, IOState> f_arg_var_type;
    typedef arg_stream_t<Char, IOState> stream_type;

private:
    f_arg_var_type m_arg;

public:
    arg_io(f_arg_type arg) : m_arg(arg) {}
    stream_type& operator()(stream_type& stream)
    {
        if SF_IF_CONSTEXPR (IOState == output)
        {
            return stream << m_arg;
        }
        else if SF_IF_CONSTEXPR (IOState == input)
        {
            return stream >> m_arg;
        }
        else
        {
            return {};
        }
    }
};

//Input/Output string slice.
template <typename Char, io_state>
class string_view_io;
template <typename Char>
class string_view_io<Char, output>
{
public:
    typedef arg_stream_t<Char, output> stream_type;

private:
    std::basic_string_view<Char> arg;

public:
    string_view_io(std::basic_string_view<Char> arg) : arg(arg) {}
    stream_type& operator()(stream_type& os)
    {
        if (!arg.empty())
        {
            return os.write(&arg.front(), arg.length());
        }
        else
        {
            return os;
        }
    }
};
template <typename Char>
class string_view_io<Char, input>
{
public:
    typedef arg_stream_t<Char, input> stream_type;

private:
    std::basic_string_view<Char> arg;

public:
    string_view_io(std::basic_string_view<Char> arg) : arg(arg) {}
    stream_type& operator()(stream_type& is)
    {
        if (!arg.empty() && is)
        {
            for (const Char& c : arg)
            {
                if (c == ' ')
                {
                    while (true)
                    {
                        char t = is.peek();
                        if (t == ' ' || t == '\t' || t == '\r' || t == '\n')
                        {
                            if (!is.get())
                                return is;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    while (is.peek() != c)
                    {
                        if (!is.get())
                            return is;
                    }
                    if (!is.get())
                        return is;
                }
            }
        }
        return is;
    }
};

template <typename Char>
SF_CONSTEXPR Char left_brace() SF_NOEXCEPT;
template <>
SF_CONSTEXPR char left_brace() SF_NOEXCEPT
{
    return '{';
}
template <>
SF_CONSTEXPR wchar_t left_brace() SF_NOEXCEPT
{
    return L'{';
}

template <typename Char>
SF_CONSTEXPR const Char* left_double_brace() SF_NOEXCEPT;
template <>
SF_CONSTEXPR const char* left_double_brace() SF_NOEXCEPT
{
    return "{{";
}
template <>
SF_CONSTEXPR const wchar_t* left_double_brace() SF_NOEXCEPT
{
    return L"{{";
}

template <typename Char>
SF_CONSTEXPR Char right_brace() SF_NOEXCEPT;
template <>
SF_CONSTEXPR char right_brace() SF_NOEXCEPT
{
    return '}';
}
template <>
SF_CONSTEXPR wchar_t right_brace() SF_NOEXCEPT
{
    return L'}';
}

template <typename Char>
SF_CONSTEXPR const Char* right_double_brace() SF_NOEXCEPT;
template <>
SF_CONSTEXPR const char* right_double_brace() SF_NOEXCEPT
{
    return "}}";
}
template <>
SF_CONSTEXPR const wchar_t* right_double_brace() SF_NOEXCEPT
{
    return L"}}";
}

//A flag indicates the type of current clice.
enum format_string_token
{
    text,
    number,
    brace
};
//A pack of format string and arguments.
template <typename Char, io_state IOState>
class format_string_view
{
public:
    typedef arg_t<Char, IOState> arg_type;
    typedef arg_list_t<Char, IOState> arg_list_type;

private:
    const std::basic_string_view<Char>& fmt;
    arg_list_type args;
    std::size_t offset, index;
    const std::size_t length;
    format_string_token state;

public:
    SF_CONSTEXPR format_string_view(const std::basic_string_view<Char>& fmt, arg_list_type&& args)
        : fmt(fmt), args(move(args)), offset(0), index(0), length(fmt.length()), state(text)
    {
    }
    arg_type move_next()
    {
        if (offset < length)
        {
            switch (state)
            {
            case text:
                index = fmt.find(left_brace<Char>(), offset);
                if (index != std::basic_string_view<Char>::npos)
                {
                    if (index + 1 < length)
                    {
                        if (fmt[index + 1] == left_brace<Char>())
                        {
                            state = brace;
                            std::size_t off = offset;
                            std::size_t len = index - offset + 1;
                            offset = index + 2;
                            return string_view_io<Char, IOState>(fmt.substr(off, len));
                        }
                    }
                    state = number;
                    std::size_t off = offset;
                    std::size_t len = index - offset;
                    offset = index + 1;
                    return string_view_io<Char, IOState>(fmt.substr(off, len));
                }
                else
                {
                    std::size_t off = offset;
                    std::size_t len = length - offset;
                    offset = length;
                    return string_view_io<Char, IOState>(fmt.substr(off, len));
                }
                break;
            case number:
                index = fmt.find(right_brace<Char>(), offset);
                if (index == std::basic_string_view<Char>::npos)
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
                if (index == std::basic_string_view<Char>::npos)
                {
                    throw std::out_of_range("No \"}}\" found after \"{{\".");
                }
                else
                {
                    state = text;
                    std::size_t off = offset;
                    std::size_t len = index - offset + 1;
                    offset = index + 2;
                    return string_view_io<Char, IOState>(fmt.substr(off, len));
                }
                break;
            }
        }
        return {};
    }
};

//Iterates the format string and arguments.
template <typename Char, io_state IOState>
SF_CONSTEXPR arg_stream_t<Char, IOState>& format(arg_stream_t<Char, IOState>& stream, format_string_view<Char, IOState> fmt)
{
    arg_t<Char, IOState> arg;
    while (arg = fmt.move_next())
    {
        arg(stream);
    }
    return stream;
}
template <typename Char, typename... Args>
SF_CONSTEXPR arg_stream_t<Char, output>& print(arg_stream_t<Char, output>& stream, const std::basic_string_view<Char>& fmt, Args&&... args)
{
    return format(stream, format_string_view<Char, output>(fmt, arg_list_t<Char, output>{ arg_io<Char, Args, output>(static_cast<forward_output_arg_t<Args>>(args))... }));
}
template <typename Char, typename... Args>
SF_CONSTEXPR arg_stream_t<Char, input>& scan(arg_stream_t<Char, input>& stream, const std::basic_string_view<Char>& fmt, Args&... args)
{
    return format(stream, format_string_view<Char, input>(fmt, arg_list_t<Char, input>{ arg_io<Char, Args, input>(static_cast<forward_input_arg_t<Args>>(args))... }));
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
