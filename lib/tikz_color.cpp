#include "tikz_color.h"

namespace TIKZ {
    color color::mix( color p_other, u64 p_amount ) const {
        return color{ this->m_name + color::MIX_SYMBOL + std::to_string( p_amount )
                      + color::MIX_SYMBOL + p_other.m_name };
    }

    color color::lighten( u64 p_amount ) const {
        return this->mix( COLOR_WHITE, p_amount );
    }

    color color::darken( u64 p_amount ) const {
        return this->mix( COLOR_BLACK, p_amount );
    }

    color::operator std::string( ) const {
        return to_string( );
    }

    void color::replace_if_non_empty( const color& p_other ) {
        if( p_other.is_non_empty( ) ) { m_name = p_other.m_name; }
    }

    bool color::is_non_empty( ) const {
        return m_name != EMPTY_STR;
    }

    color color::to_flavor_bg( ) const {
        return lighten( 4 );
    }
    color color::to_bg( ) const {
        return lighten( 10 );
    }

    color color::deemphasize_strong( ) const {
        return lighten( 25 );
    }
    color color::deemphasize( ) const {
        return lighten( 50 );
    }
    color color::deemphasize_weak( ) const {
        return lighten( 75 );
    }

    const char* color::c_str( ) const {
        return m_name.c_str( );
    }

    std::string color::to_string( ) const {
        return m_name;
    }
} // namespace TIKZ
