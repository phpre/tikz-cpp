#pragma once
#include <string>
#include "defines.h"

namespace TIKZ {
    struct color {
      private:
        std::string m_name;

        static constexpr std::string MIX_SYMBOL = "!";

      public:
        constexpr color( const std::string& p_name ) : m_name{ p_name } {
        }

        color       mix( color p_other, u64 p_amount ) const;
        color       lighten( u64 p_amount ) const;
        color       darken( u64 p_amount ) const;
                    operator std::string( ) const;
        void        replace_if_non_empty( const color& p_other );
        bool        is_non_empty( ) const;
        color       to_flavor_bg( ) const;
        color       to_bg( ) const;
        color       deemphasize_strong( ) const;
        color       deemphasize( ) const;
        color       deemphasize_weak( ) const;
        const char* c_str( ) const;
        std::string to_string( ) const;
    };

    constexpr color COLOR_NONE{ "" };
    constexpr color COLOR_BLACK{ "black" };
    constexpr color COLOR_TEXT{ "black!80!white" };
    constexpr color COLOR_WHITE{ "white" };
    constexpr color COLOR_FILL_WHITE{ "black!5!white" };

    constexpr color COLOR_C1{ "c1" };
    constexpr color COLOR_C2{ "c2" };
    constexpr color COLOR_C3{ "c3" };
    constexpr color COLOR_C4{ "c4" };
    constexpr color COLOR_C5{ "c5" };
} // namespace TIKZ
