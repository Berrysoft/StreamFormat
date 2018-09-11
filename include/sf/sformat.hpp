// StreamFormat sformat.hpp
#ifndef SF_SFORMAT_HPP
#define SF_SFORMAT_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX17) || _MSC_VER >= 1910

#include <sf/format.hpp>
#include <sstream>

namespace sf
{
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR std::basic_string<Char, Traits, Alloc> sscan(const std::basic_string<Char, Traits, Alloc>& str, std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        std::basic_istringstream<Char, Traits, Alloc> iss(str);
        scan(iss, fmt, std::forward<Args>(args)...);
        if (!iss.eof())
            return str.substr(iss.tellg());
        else
            return {};
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR std::basic_string<Char, Traits, Alloc> sprint(std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        std::basic_ostringstream<Char, Traits, Alloc> oss;
        print(oss, fmt, std::forward<Args>(args)...);
        return oss.str();
    }
} // namespace sf

#endif // SF_CXX17

#endif // !SF_SFORMAT_HPP
