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
        constexpr typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, const std::basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            std::basic_istringstream<Char, Traits, Allocator> iss(str);
            format<input>(iss, fmt, std::forward<Args>(args)...);
            return iss.tellg();
        }
        template <typename Char, typename Traits, typename Allocator, typename T>
        constexpr typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, T&& arg)
        {
            std::basic_istringstream<Char, Traits, Allocator> iss(str);
            format<input, Char, Traits, T>(iss, std::forward<T>(arg));
            return iss.tellg();
        }

        template <typename Char, typename Traits, typename Allocator, typename... Args>
        constexpr std::basic_string<Char, Traits, Allocator> sprint(const std::basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            std::basic_ostringstream<Char, Traits, Allocator> oss;
            format<output>(oss, fmt, std::forward<Args>(args)...);
            return oss.str();
        }
        template <typename Char, typename Traits, typename Allocator, typename T>
        constexpr std::basic_string<Char, Traits, Allocator> sprint(T&& arg)
        {
            std::basic_ostringstream<Char, Traits, Allocator> oss;
            format<output, Char, Traits, T>(oss, std::forward<T>(arg));
            return oss.str();
        }
    } // namespace internal

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename... Args>
    constexpr typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::sscan<Char, Traits, Allocator>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename... Args>
    constexpr std::basic_string<Char, Traits, Allocator> sprint(std::basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::sprint<Char, Traits, Allocator>(fmt, std::forward<Args>(args)...);
    }

    //clang will error if the return types are written explicitly.
    //`auto` will make it happy.

    //char IO
    template <typename... Args>
    constexpr auto sscan(const std::string& str, std::string_view fmt, Args&&... args)
    {
        return internal::sscan<char, std::char_traits<char>, std::allocator<char>>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    constexpr auto sprint(std::string_view fmt, Args&&... args)
    {
        return internal::sprint<char, std::char_traits<char>, std::allocator<char>>(fmt, std::forward<Args>(args)...);
    }

    //wchar IO
    template <typename... Args>
    constexpr auto sscan(const std::wstring& str, std::wstring_view fmt, Args&&... args)
    {
        return internal::sscan<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    constexpr auto sprint(std::wstring_view fmt, Args&&... args)
    {
        return internal::sprint<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(fmt, std::forward<Args>(args)...);
    }

    //Simple IO functions for convinence.

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename T>
    constexpr typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, T&& arg)
    {
        return internal::sscan<Char, Traits, Allocator, T>(str, std::forward<T>(arg));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename T>
    constexpr std::basic_string<Char, Traits, Allocator> sprint(T&& arg)
    {
        return internal::sprint<Char, Traits, Allocator, T>(std::forward<T>(arg));
    }

#if !SF_FORCE_WIDE_IO
    //char IO
    template <typename T>
    constexpr auto sscan(const std::string& str, T&& arg)
    {
        return internal::sscan<char, std::char_traits<char>, std::allocator<char>, T>(str, std::forward<T>(arg));
    }
    template <typename T>
    constexpr auto sprint(T&& arg)
    {
        return internal::sprint<char, std::char_traits<char>, std::allocator<char>, T>(std::forward<T>(arg));
    }
#else
    //wchar_t IO
    template <typename T>
    constexpr auto sscan(const std::wstring& str, T&& arg)
    {
        return internal::sscan<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>, T>(str, std::forward<T>(arg));
    }
    template <typename T>
    constexpr auto sprint(T&& arg)
    {
        return internal::sprint<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>, T>(std::forward<T>(arg));
    }
#endif // !SF_FORCE_WIDE_IO
} // namespace sf

#endif // !SF_SFORMAT_HPP
