#ifndef SF_UTILITY_HPP
#define SF_UTILITY_HPP

#if !defined(SF_CXX11) && __cplusplus >= 201103L
#define SF_CXX11
#endif // C++11

#if !defined(SF_CXX14) && __cplusplus >= 201402L
#define SF_CXX14
#endif // C++14

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

#ifndef SF_IF_CONSTEXPR
#if defined(SF_CXX17) || _MSC_VER >= 1911
#define SF_IF_CONSTEXPR if constexpr
#else
#define SF_IF_CONSTEXPR if
#endif // SF_CXX17
#endif // !SF_IF_CONSTEXPR

#ifndef SF_NOEXCEPT
#if defined(SF_CXX11) || _MSC_VER >= 1900
#define SF_NOEXCEPT noexcept
#else
#define SF_NOEXCEPT throw()
#endif // SF_CXX11
#endif // !SF_NOEXCEPT

#ifndef SF_CHAR_TEMPLATE
#define SF_CHAR_TEMPLATE(name, value)       \
    template <typename Char>                \
    SF_CONSTEXPR Char name() SF_NOEXCEPT;   \
    template <>                             \
    SF_CONSTEXPR char name() SF_NOEXCEPT    \
    {                                       \
        return value;                       \
    }                                       \
    template <>                             \
    SF_CONSTEXPR wchar_t name() SF_NOEXCEPT \
    {                                       \
        return L##value;                    \
    }
#endif // !SF_CHAR_TEMPLATE

#ifndef SF_STR_TEMPLATE
#define SF_STR_TEMPLATE(name, value)               \
    template <typename Char>                       \
    SF_CONSTEXPR const Char* name() SF_NOEXCEPT;   \
    template <>                                    \
    SF_CONSTEXPR const char* name() SF_NOEXCEPT    \
    {                                              \
        return value;                              \
    }                                              \
    template <>                                    \
    SF_CONSTEXPR const wchar_t* name() SF_NOEXCEPT \
    {                                              \
        return L##value;                           \
    }
#endif // !SF_STR_TEMPLATE

#endif // !SF_UTILITY_HPP
