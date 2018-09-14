// StreamFormat sformat.hpp
#ifndef SF_SFORMAT_HPP
#define SF_SFORMAT_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX17) || _MSC_VER >= 1910

#include <sf/format.hpp>
#include <sstream>

namespace sf
{
    namespace internal
    {
        template <typename Char, typename Traits, typename Allocator, typename... Args>
        SF_CONSTEXPR typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, const std::basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            std::basic_istringstream<Char, Traits, Allocator> iss(str);
            format<input>(iss, fmt, std::forward<Args>(args)...);
            return iss.tellg();
        }
        template <typename Char, typename Traits, typename Allocator, typename... Args>
        SF_CONSTEXPR std::basic_string<Char, Traits, Allocator> sprint(const std::basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            std::basic_ostringstream<Char, Traits, Allocator> oss;
            format<output>(oss, fmt, std::forward<Args>(args)...);
            return oss.str();
        }
    } // namespace internal

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::sscan<Char, Traits, Allocator>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR std::basic_string<Char, Traits, Allocator> sprint(std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::sprint<Char, Traits, Allocator>(fmt, std::forward<Args>(args)...);
    }

    //clang will error if the return types are written explicitly.
    //`auto` will make it happy.

    //char IO
    template <typename... Args>
    SF_CONSTEXPR auto sscan(const std::string& str, std::string_view fmt, Args&&... args)
    {
        return internal::sscan<char, std::char_traits<char>, std::allocator<char>>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR auto sprint(std::string_view fmt, Args&&... args)
    {
        return internal::sprint<char, std::char_traits<char>, std::allocator<char>>(fmt, std::forward<Args>(args)...);
    }

    //wchar IO
    template <typename... Args>
    SF_CONSTEXPR auto sscan(const std::wstring& str, std::wstring_view fmt, Args&&... args)
    {
        return internal::sscan<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR auto sprint(std::wstring_view fmt, Args&&... args)
    {
        return internal::sprint<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(fmt, std::forward<Args>(args)...);
    }
} // namespace sf

#endif // SF_CXX17

#endif // !SF_SFORMAT_HPP
