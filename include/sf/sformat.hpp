/**StreamFormat sformat.hpp
 * 
 * MIT License
 * 
 * Copyright (c) 2018-2019 Berrysoft
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
#ifndef SF_SFORMAT_HPP
#define SF_SFORMAT_HPP

#include <sf/utility.hpp>

#include <sf/format.hpp>
#include <sstream>

namespace sf
{
    namespace internal
    {
        template <typename Char, typename Traits, typename Allocator, typename... Args>
        constexpr typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, std::basic_string_view<Char, Traits> fmt, Args&&... args)
        {
            std::basic_istringstream<Char, Traits, Allocator> iss(str);
            format<input>(iss, fmt, std::forward<Args>(args)...);
            return iss.tellg();
        }
        template <typename Char, typename Traits, typename Allocator, typename... Args>
        constexpr std::basic_string<Char, Traits, Allocator> sprint(std::basic_string_view<Char, Traits> fmt, Args&&... args)
        {
            std::basic_ostringstream<Char, Traits, Allocator> oss;
            format<output>(oss, fmt, std::forward<Args>(args)...);
            return oss.str();
        }
    } // namespace internal

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename... Args, typename Str, typename = std::enable_if_t<std::is_convertible_v<Str, std::basic_string<Char, Traits, Allocator>>>, typename String, typename = std::enable_if_t<std::is_convertible_v<String, std::basic_string_view<Char, Traits>>>>
    constexpr typename Traits::pos_type sscan(Str&& str, String&& fmt, Args&&... args)
    {
        return internal::sscan<Char, Traits, Allocator>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename String, typename... Args, typename = std::enable_if_t<std::is_convertible_v<String, std::basic_string_view<Char, Traits>>>>
    constexpr std::basic_string<Char, Traits, Allocator> sprint(String&& fmt, Args&&... args)
    {
        return internal::sprint<Char, Traits, Allocator>(fmt, std::forward<Args>(args)...);
    }

    //char IO
    template <typename... Args>
    constexpr auto sscan(const std::string& str, std::string_view fmt, Args&&... args)
    {
        return sscan<char>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    constexpr auto sprint(std::string_view fmt, Args&&... args)
    {
        return sprint<char>(fmt, std::forward<Args>(args)...);
    }

    //wchar_t IO
    template <typename... Args>
    constexpr auto wsscan(const std::wstring& str, std::wstring_view fmt, Args&&... args)
    {
        return sscan<wchar_t>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    constexpr auto wsprint(std::wstring_view fmt, Args&&... args)
    {
        return sprint<wchar_t>(fmt, std::forward<Args>(args)...);
    }

    //char16_t IO
    template <typename... Args>
    constexpr auto u16sscan(const std::u16string& str, std::u16string_view fmt, Args&&... args)
    {
        return sscan<char16_t>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    constexpr auto u16sprint(std::u16string_view fmt, Args&&... args)
    {
        return sprint<char16_t>(fmt, std::forward<Args>(args)...);
    }

    //char32_t IO
    template <typename... Args>
    constexpr auto u16sscan(const std::u32string& str, std::u32string_view fmt, Args&&... args)
    {
        return sscan<char32_t>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    constexpr auto u16sprint(std::u32string_view fmt, Args&&... args)
    {
        return sprint<char32_t>(fmt, std::forward<Args>(args)...);
    }
} // namespace sf

#endif // !SF_SFORMAT_HPP
