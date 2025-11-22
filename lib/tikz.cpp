#include "old.h"
#include "tikz.h"
#include "tikz_document.h"

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

    void print_connection( FILE* p_out, const std::string& p_topLeft,
                           const std::string& p_bottomRight, const std::string& p_connection,
                           const std::string& p_options, u64 p_startIndent, u64 p_indent ) {
        INDENT_PRINT( p_startIndent )( p_out, "\\draw[%s] (%s) %s (%s);\n", p_options.c_str( ),
                                       p_topLeft.c_str( ), p_connection.c_str( ),
                                       p_bottomRight.c_str( ) );
    }

    void print_connection( FILE* p_out, tikz_point p_topLeft, tikz_point p_bottomRight,
                           const std::string& p_connection, const std::string& p_options,
                           u64 p_startIndent, u64 p_indent ) {
        char buffer1[ 30 ] = { 0 };
        char buffer2[ 30 ] = { 0 };
        snprintf( buffer1, 29, "%5.3lf, %5.3lf", p_topLeft.m_x, p_topLeft.m_y );
        snprintf( buffer2, 29, "%5.3lf, %5.3lf", p_bottomRight.m_x, p_bottomRight.m_y );
        print_connection( p_out, buffer1, buffer2, p_connection, p_options, p_startIndent,
                          p_indent );
    }

} // namespace TIKZ
