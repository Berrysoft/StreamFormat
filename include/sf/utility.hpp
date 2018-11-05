/**StreamFormat utility.hpp
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
#ifndef SF_UTILITY_HPP
#define SF_UTILITY_HPP

#if !defined(SF_CXX11) && __cplusplus >= 201103L || _MSVC_LANG >= 201103L
#define SF_CXX11
#endif // C++11

#if !defined(SF_CXX14) && __cplusplus >= 201402L || _MSVC_LANG >= 201402L
#define SF_CXX14
#endif // C++14

#if !defined(SF_CXX17) && __cplusplus >= 201703L || _MSVC_LANG >= 201703L
#define SF_CXX17
#endif // C++17

#ifndef SF_CONSTEXPR
#if defined(SF_CXX14) || _MSC_VER >= 1910
#define SF_CONSTEXPR constexpr
#define SF_CONSTEXPR_DECL constexpr
#elif defined(SF_CXX11) || _MSC_VER >= 1900
#define SF_CONSTEXPR inline
#define SF_CONSTEXPR_DECL constexpr
#else
#define SF_CONSTEXPR inline
#define SF_CONSTEXPR_DECL const
#endif // SF_CXX14
#endif // !SF_CONSTEXPR

#ifndef SF_NOEXCEPT
#if defined(SF_CXX11) || _MSC_VER >= 1900
#define SF_NOEXCEPT noexcept
#else
#define SF_NOEXCEPT throw()
#endif // SF_CXX11
#endif // !SF_NOEXCEPT

#ifndef SF_NULLPTR
#if defined(SF_CXX11) || _MSC_VER >= 1600
#define SF_NULLPTR nullptr
#else
#define SF_NULLPTR 0
#endif // SF_CXX11
#endif // !SF_NULLPTR

#if defined(_MSC_VER) && defined(_UNICODE) && !defined(SF_FORCE_WIDE_IO)
#define SF_FORCE_WIDE_IO 1
#endif // !SF_FORCE_WIDE_IO

#ifndef SF_CHAR_TEMPLATE
#define SF_CHAR_TEMPLATE(name, value)        \
    template <typename Char>                 \
    SF_CONSTEXPR Char name() SF_NOEXCEPT;    \
    template <>                              \
    SF_CONSTEXPR char name() SF_NOEXCEPT     \
    {                                        \
        return value;                        \
    }                                        \
    template <>                              \
    SF_CONSTEXPR char16_t name() SF_NOEXCEPT \
    {                                        \
        return u##value;                     \
    }                                        \
    template <>                              \
    SF_CONSTEXPR char32_t name() SF_NOEXCEPT \
    {                                        \
        return U##value;                     \
    }                                        \
    template <>                              \
    SF_CONSTEXPR wchar_t name() SF_NOEXCEPT  \
    {                                        \
        return L##value;                     \
    }
#endif // !SF_CHAR_TEMPLATE

#endif // !SF_UTILITY_HPP
