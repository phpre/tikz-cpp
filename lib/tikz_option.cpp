#include "tikz_color.h"
#include "tikz_option.h"

namespace TIKZ {
    std::string tikz_option::to_string( ) const {
        auto res = m_name;
        if( m_value == EMPTY_STR && m_defaultValue == EMPTY_STR ) { return res; }
        res += " = ";
        if( m_value == EMPTY_STR ) { return res + m_defaultValue; }
        return res + m_value;
    }

    tikz_option tikz_option::operator( )( const std::string& p_value ) const {
        return tikz_option{ m_name, m_defaultValue, p_value, m_libraries, m_packages };
    }

    tikz_option::operator kv_store( ) const {
        return kv_store{ { *this } };
    }

    kv_store tikz_option::operator|( const tikz_option& p_other ) const {
        return kv_store{ { *this, p_other } };
    }

    kv_store tikz_option::operator|( const kv_store& p_other ) const {
        return kv_store{ { *this } } | p_other;
    }

} // namespace TIKZ
