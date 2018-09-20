//StreamFormat stoi.hpp
#ifndef SF_STOI_HPP
#define SF_STOI_HPP

#include <sf/utility.hpp>

#if defined(SF_CXX17) || _MSC_VER >= 1910

#include <string>

#ifndef SF_STOI_TEMPLATE
#define SF_STOI_TEMPLATE(name, type)                                                 \
    template <typename Char, typename Traits>                                        \
    SF_CONSTEXPR type name(const std::basic_string_view<Char, Traits>& str)          \
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
    }
}
#endif // SF_CXX17

#endif // !SF_STOI_HPP
