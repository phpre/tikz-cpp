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

    void print_coordinate( FILE* p_out, const std::string& p_position, const std::string& p_name,
                           const std::string& p_options, u64 p_startIndent, u64 p_indent ) {
        INDENT_PRINT( p_startIndent )( p_out, "\\coordinate[%s] (%s) at (%s);\n",
                                       p_options.c_str( ), p_name.c_str( ), p_position.c_str( ) );
    }

    void print_coordinate( FILE* p_out, tikz_point p_position, const std::string& p_name,
                           const std::string& p_options, u64 p_startIndent, u64 p_indent ) {
        char buffer[ 30 ] = { 0 };
        snprintf( buffer, 29, "%5.3lf, %5.3lf", p_position.m_x, p_position.m_y );
        print_coordinate( p_out, std::string{ buffer }, p_name, p_options, p_startIndent,
                          p_indent );
    }

    void print_node( FILE* p_out, const std::string& p_position, const std::string& p_content,
                     const std::string& p_options, const std::string& p_name, u64 p_startIndent,
                     u64 p_indent ) {
        if( p_name == EMPTY_STR ) {
            INDENT_PRINT( p_startIndent )( p_out, "\\node[%s] at (%s) {%s};\n", p_options.c_str( ),
                                           p_position.c_str( ), p_content.c_str( ) );
        } else {
            INDENT_PRINT( p_startIndent )( p_out, "\\node[%s] (%s) at (%s) {%s};\n",
                                           p_options.c_str( ), p_name.c_str( ), p_position.c_str( ),
                                           p_content.c_str( ) );
        }
    }

    void print_node( FILE* p_out, tikz_point p_position, const std::string& p_content,
                     const std::string& p_options, const std::string& p_name, u64 p_startIndent,
                     u64 p_indent ) {
        char buffer[ 30 ] = { 0 };
        snprintf( buffer, 29, "%5.3lf, %5.3lf", p_position.m_x, p_position.m_y );
        print_node( p_out, std::string{ buffer }, p_content, p_options, p_name, p_startIndent,
                    p_indent );
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

    void print_debug_point( FILE* p_out, tikz_point p_position, u64 p_startIndent, u64 p_indent ) {
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[fill, circle, red, inner sep = 1pt] at (%5.3lf, %5.3lf) {};\n",
            p_position.m_x, p_position.m_y );
    }

    std::pair<tikz_point, tikz_point> translate_right( std::pair<tikz_point, tikz_point> p_pos,
                                                       double p_amount ) {
        return { tikz_point{ p_pos.first.m_x + p_amount, p_pos.first.m_y },
                 tikz_point{ p_pos.second.m_x + p_amount, p_pos.second.m_y } };
    }

    std::pair<tikz_point, tikz_point> translate_down( std::pair<tikz_point, tikz_point> p_pos,
                                                      double                            p_amount ) {
        return { tikz_point{ p_pos.first.m_x, p_amount + p_pos.first.m_y },
                 tikz_point{ p_pos.second.m_x, p_amount + p_pos.second.m_y } };
    }

} // namespace TIKZ
