/**StreamFormat stoi.hpp
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
#ifndef SF_STOI_HPP
#define SF_STOI_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX11)

#include <sf/string_view.hpp>
#include <string>

#ifndef SF_STOI_TEMPLATE
#define SF_STOI_TEMPLATE(name, type)                                                 \
    template <typename Char, typename Traits>                                        \
    SF_CONSTEXPR type name(const basic_string_view<Char, Traits>& str)               \
    {                                                                                \
        return std::name(std::basic_string<Char, Traits>(str.data(), str.length())); \
    }
#endif // !SF_STOI_TEMPLATE

namespace sf
{
    namespace internal
    {
        SF_STOI_TEMPLATE(stoi, int)
        SF_STOI_TEMPLATE(stol, long)
        SF_STOI_TEMPLATE(stoll, long long)
        SF_STOI_TEMPLATE(stoul, unsigned long)
        SF_STOI_TEMPLATE(stoull, unsigned long long)
    } // namespace internal
} // namespace sf
#endif // SF_CXX11

#endif // !SF_STOI_HPP
