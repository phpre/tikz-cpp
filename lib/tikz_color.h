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

        color mix( color p_other, u32 p_amount ) const;
        color lighten( u32 p_amount ) const;
        color darken( u32 p_amount ) const;

        inline void replace_if_non_empty( const color& p_other ) {
            if( p_other.is_non_empty( ) ) { m_name = p_other.m_name; }
        }

        inline bool is_non_empty( ) const {
            return m_name != EMPTY_STR;
        }

        inline color to_flavor_bg( ) const {
            return lighten( 4 );
        }
        inline color to_bg( ) const {
            return lighten( 10 );
        }

        inline color deemphasize_strong( ) const {
            return lighten( 25 );
        }
        inline color deemphasize( ) const {
            return lighten( 50 );
        }
        inline color deemphasize_weak( ) const {
            return lighten( 75 );
        }

        const char* c_str( ) const {
            return m_name.c_str( );
        }

        std::string to_string( ) const {
            return m_name;
        }
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
