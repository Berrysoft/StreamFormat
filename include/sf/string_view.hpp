/**StreamFormat string_view.hpp
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
#ifndef SF_STRING_VIEW_HPP
#define SF_STRING_VIEW_HPP

#include <sf/utility.hpp>

#include <string>

namespace sf
{
    template <typename Char, typename Traits = std::char_traits<Char>>
    class basic_string_view
    {
    public:
        typedef Traits traits_type;
        typedef Char value_type;
        typedef const Char* const_pointer;
        typedef const_pointer iterator;
        typedef const Char& const_reference;
        typedef std::size_t size_type;

    private:
        const_pointer m_start;
        size_type m_size;

    public:
        static SF_CONSTEXPR_DECL size_type npos = size_type(-1);

        SF_CONSTEXPR const_pointer data() const SF_NOEXCEPT { return m_start; }
        SF_CONSTEXPR size_type length() const SF_NOEXCEPT { return m_size; }

        SF_CONSTEXPR basic_string_view() SF_NOEXCEPT : m_start(SF_NULLPTR), m_size(0) {}
        SF_CONSTEXPR basic_string_view(const basic_string_view<Char, Traits>&) SF_NOEXCEPT = default;
        SF_CONSTEXPR basic_string_view(const_pointer s, size_type count) : m_start(s), m_size(count) {}
        SF_CONSTEXPR basic_string_view(const_pointer s) : m_start(s), m_size(Traits::length(s)) {}
        template <typename Allocator = std::allocator<Char>>
        SF_CONSTEXPR basic_string_view(const std::basic_string<Char, Traits, Allocator>& s) : m_start(s), m_size(s.length())
        {
        }

#ifdef SF_CXX17
        SF_CONSTEXPR basic_string_view(const std::basic_string_view<Char, Traits>& s) : m_start(s.data()), m_size(s.length())
        {
        }
#endif // SF_CXX17

        SF_CONSTEXPR const_reference operator[](size_type index) const
        {
            return m_start[index];
        }

        SF_CONSTEXPR bool empty() const SF_NOEXCEPT { return m_size == 0; }

        SF_CONSTEXPR basic_string_view<Char, Traits> substr(size_type pos = 0, size_type count = npos) const
        {
            //To refuce use of <algorithm>.
            size_type maxlen = m_size - pos;
            return basic_string_view<Char, Traits>(m_start + pos, count > maxlen ? maxlen : count);
        }

        SF_CONSTEXPR iterator begin() const SF_NOEXCEPT { return m_start; }
        SF_CONSTEXPR iterator end() const SF_NOEXCEPT { return m_start + m_size; }
    };

    template <typename Char, typename Traits>
    SF_CONSTEXPR std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, const basic_string_view<Char, Traits>& str)
    {
        return str.empty() ? stream : stream.write(str.data(), str.length());
    }

    typedef basic_string_view<char> string_view;
    typedef basic_string_view<wchar_t> wstring_view;
} // namespace sf

#endif // !SF_STRING_VIEW_HPP
