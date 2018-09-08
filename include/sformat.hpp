#ifndef SF_SFORMAT_HPP
#define SF_SFORMAT_HPP

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

#if defined(SF_CXX17) || _MSC_VER >= 1910

#include "format.hpp"
#include <sstream>

namespace sf
{
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR std::basic_string<Char, Traits, Alloc> sscan(const std::basic_string<Char, Traits, Alloc>& str, std::basic_string_view<Char, Traits> fmt, Args&... args)
    {
        std::basic_istringstream<Char, Traits, Alloc> iss(str);
        scan(iss, fmt, args...);
        if (!iss.eof())
            return str.substr(iss.tellg());
        else
            return {};
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR std::basic_string<Char, Traits, Alloc> sprint(std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        std::basic_ostringstream<Char, Traits, Alloc> oss;
        print(oss, fmt, args...);
        return oss.str();
    }
} // namespace sf

#endif // SF_CXX17

#endif // !SF_SFORMAT_HPP