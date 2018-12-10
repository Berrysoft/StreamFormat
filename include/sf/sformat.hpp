/**StreamFormat sformat.hpp
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
#ifndef SF_SFORMAT_HPP
#define SF_SFORMAT_HPP

#include <sf/utility.hpp>

#ifdef SF_CXX11

#include <sf/format.hpp>
#include <sstream>

namespace sf
{
    namespace internal
    {
        template <typename Char, typename Traits, typename Allocator, typename... Args>
        SF_CONSTEXPR typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, const basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            std::basic_istringstream<Char, Traits, Allocator> iss(str);
            format<input>(iss, fmt, std::forward<Args>(args)...);
            return iss.tellg();
        }
        template <typename Char, typename Traits, typename Allocator, typename T>
        SF_CONSTEXPR typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, T&& arg)
        {
            std::basic_istringstream<Char, Traits, Allocator> iss(str);
            format<input, Char, Traits, T>(iss, std::forward<T>(arg));
            return iss.tellg();
        }

        template <typename Char, typename Traits, typename Allocator, typename... Args>
        SF_CONSTEXPR std::basic_string<Char, Traits, Allocator> sprint(const basic_string_view<Char, Traits>& fmt, Args&&... args)
        {
            std::basic_ostringstream<Char, Traits, Allocator> oss;
            format<output>(oss, fmt, std::forward<Args>(args)...);
            return oss.str();
        }
        template <typename Char, typename Traits, typename Allocator, typename T>
        SF_CONSTEXPR std::basic_string<Char, Traits, Allocator> sprint(T&& arg)
        {
            std::basic_ostringstream<Char, Traits, Allocator> oss;
            format<output, Char, Traits, T>(oss, std::forward<T>(arg));
            return oss.str();
        }
    } // namespace internal

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::sscan<Char, Traits, Allocator>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename... Args>
    SF_CONSTEXPR std::basic_string<Char, Traits, Allocator> sprint(basic_string_view<Char, Traits> fmt, Args&&... args)
    {
        return internal::sprint<Char, Traits, Allocator>(fmt, std::forward<Args>(args)...);
    }

    //clang will error if the return types are written explicitly.
    //`auto` will make it happy.
    //And must specify the type because C++11 is a little fool.

    //char IO
    template <typename... Args>
    SF_CONSTEXPR auto sscan(const std::string& str, string_view fmt, Args&&... args)
        -> decltype(internal::sscan<char, std::char_traits<char>, std::allocator<char>>(str, fmt, std::forward<Args>(args)...))
    {
        return internal::sscan<char, std::char_traits<char>, std::allocator<char>>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR auto sprint(string_view fmt, Args&&... args)
        -> decltype(internal::sprint<char, std::char_traits<char>, std::allocator<char>>(fmt, std::forward<Args>(args)...))
    {
        return internal::sprint<char, std::char_traits<char>, std::allocator<char>>(fmt, std::forward<Args>(args)...);
    }

    //wchar IO
    template <typename... Args>
    SF_CONSTEXPR auto sscan(const std::wstring& str, wstring_view fmt, Args&&... args)
        -> decltype(internal::sscan<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(str, fmt, std::forward<Args>(args)...))
    {
        return internal::sscan<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(str, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    SF_CONSTEXPR auto sprint(wstring_view fmt, Args&&... args)
        -> decltype(internal::sprint<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(fmt, std::forward<Args>(args)...))
    {
        return internal::sprint<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(fmt, std::forward<Args>(args)...);
    }

    //Simple IO functions for convinence.

    //template IO
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename T>
    SF_CONSTEXPR typename Traits::pos_type sscan(const std::basic_string<Char, Traits, Allocator>& str, T&& arg)
    {
        return internal::sscan<Char, Traits, Allocator, T>(str, std::forward<T>(arg));
    }
    template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>, typename T>
    SF_CONSTEXPR std::basic_string<Char, Traits, Allocator> sprint(T&& arg)
    {
        return internal::sprint<Char, Traits, Allocator, T>(std::forward<T>(arg));
    }

#if !SF_FORCE_WIDE_IO
    //char IO
    template <typename T>
    SF_CONSTEXPR auto sscan(const std::string& str, T&& arg)
        -> decltype(internal::sscan<char, std::char_traits<char>, std::allocator<char>, T>(str, std::forward<T>(arg)))
    {
        return internal::sscan<char, std::char_traits<char>, std::allocator<char>, T>(str, std::forward<T>(arg));
    }
    template <typename T>
    SF_CONSTEXPR auto sprint(T&& arg)
        -> decltype(internal::sprint<char, std::char_traits<char>, std::allocator<char>, T>(std::forward<T>(arg)))
    {
        return internal::sprint<char, std::char_traits<char>, std::allocator<char>, T>(std::forward<T>(arg));
    }
#else
    //wchar_t IO
    template <typename T>
    SF_CONSTEXPR auto sscan(const std::wstring& str, T&& arg)
        -> decltype(internal::sscan<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>, T>(str, std::forward<T>(arg)))
    {
        return internal::sscan<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>, T>(str, std::forward<T>(arg));
    }
    template <typename T>
    SF_CONSTEXPR auto sprint(T&& arg)
        -> decltype(internal::sprint<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>, T>(std::forward<T>(arg)))
    {
        return internal::sprint<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>, T>(std::forward<T>(arg));
    }
#endif // !SF_FORCE_WIDE_IO
} // namespace sf

#endif // SF_CXX11

#endif // !SF_SFORMAT_HPP
