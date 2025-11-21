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
        return tikz_option{
            m_name,      m_defaultValue, p_value == EMPTY_STR ? m_defaultValue : p_value,
            m_libraries, m_packages,     m_styles };
    }

    kv_store tikz_option::operator|( const tikz_option& p_other ) const {
        return kv_store{ { *this, p_other } };
    }

    tikz_option::operator kv_store( ) const {
        return kv_store{ { *this } };
    }

    namespace OPT {
        const tikz_option TEXT_COLOR{ "text", COLOR_TEXT.to_string( ) };
        const tikz_option DRAW{ "draw", COLOR_TEXT.to_string( ) };
        const tikz_option FILL{ "fill", COLOR_TEXT.to_string( ) };

        const tikz_option ROTATE{ "rotate", "0" };
        const tikz_option XSCALE{ "xscale", "1" };
        const tikz_option YSCALE{ "yscale", "1" };

        const tikz_option OUTER_SEP{ "outer sep", "0pt" };
        const tikz_option INNER_SEP{ "inner sep", "0pt" };

        const tikz_option ROUNDED_CORNERS{ "rounded corners", "3pt" };

        const tikz_option LINE_WIDTH{ "line width", "1.25pt" };
        const tikz_option LW_VERY_THIN_OUTLINE{ "line width", ".5pt" };
        const tikz_option LW_THIN_OUTLINE{ "line width", ".75pt" };
        const tikz_option LW_OUTLINE{ "line width", "1pt" };
        const tikz_option LW_SUPPORT_LINE{ "line width", ".5pt" };
        const tikz_option LW_THICK_SUPPORT_LINE{ "line width", ".75pt" };

        const tikz_option ARR_TIP_LATEX{ "-latex" };
    } // namespace OPT
} // namespace TIKZ
