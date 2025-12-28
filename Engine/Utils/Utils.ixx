export module Aether:utils;

import :safe_c;
import :error;
import std;
import <string>;

export namespace Aether
{
	constexpr size_t aether_alignup(size_t x, size_t a) {
        return ( (x + a - 1) & (~(a - 1)) );
    }
    constexpr size_t aether_aligndown(size_t x, size_t a) {
        return ( x & (~(a - 1)) );
	}
    constexpr uintptr_t aether_alignaddr(void* x, size_t a) {
        return ( ( (uintptr_t)(x) + ( (uintptr_t)(a) - (uintptr_t)1 ) ) & ( (uintptr_t)-(a) ) );
	}

    // String function
    std::string                 string_trim(std::string const& str);
    std::vector<std::string>    string_split(std::string const& str, char delimiter = ',');
    void                        string_replace(std::string& str, std::string const& from, std::string const& to);

    // Type Convert
    template <typename E>
    constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

    template<typename T>
    struct default_array_deleter
    {
        void operator ()(T const* p)
        {
            delete[] p;
        }
    };

#if defined(AETHER_PLATFORM_WINDOWS)
    std::wstring utf8_to_wchar(const char* src);
#endif

};