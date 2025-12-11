#include <format>
#include "tikz_command.h"

namespace TIKZ {
    render_t path_command::render( u64 p_startIndent ) const {
        render_t result{ };

        std::string res = "\\path";
        if( !_options.empty( ) ) {
            res += "[";
            res += _options.to_string( );
            res += "]";
        }
        if( !m_startPosition.empty( ) ) { res += " (" + m_startPosition.to_string( ) + ")"; }
        result.push_back( { p_startIndent, res } );

        for( const auto& op : m_operations ) {
            result.append_range( op->render_op( 1 + p_startIndent ) );
        }

        result.push_back( { p_startIndent, ";" } );
        return result;
    }

    render_t scope_operation::render_op( u64 p_startIndent ) const {
        render_t result{ };
        result.push_back( { p_startIndent, "{" } );
        for( const auto& op : m_operations ) {
            result.append_range( op->render_op( 1 + p_startIndent ) );
        }
        result.push_back( { p_startIndent, "}" } );
        return result;
    }

    render_t arc_to_operation::render_op( u64 p_startIndent ) const {
        return { { p_startIndent, std::format( "arc ({:5.3f}:{:5.3f}:{:5.3f}pt)", m_startAngle,
                                               m_endAngle, m_radius ) } };
    }

    render_t move_to_operation::render_op( u64 p_startIndent ) const {
        std::string res{ };
        if( !_options.empty( ) ) {
            res += "[";
            res += _options.to_string( );
            res += "]";
        }
        if( !m_position.empty( ) ) {
            res += " (" + m_position.to_string( ) + ")";
        } else {
            res += " cycle";
        }

        return { { p_startIndent, res } };
    }

    render_t line_to_operation::render_op( u64 p_startIndent ) const {
        std::string res{ };
        if( !_options.empty( ) ) {
            res += "[";
            res += _options.to_string( );
            res += "]";
        }
        if( !m_position.empty( ) ) {
            res += "-- (" + m_position.to_string( ) + ")";
        } else {
            res += "-- cycle";
        }

        return { { p_startIndent, res } };
    }

    render_t hv_line_to_operation::render_op( u64 p_startIndent ) const {
        std::string res{ };
        if( !_options.empty( ) ) {
            res += "[";
            res += _options.to_string( );
            res += "]";
        }
        if( !m_position.empty( ) ) {
            res += "-| (" + m_position.to_string( ) + ")";
        } else {
            res += "-| cycle";
        }

        return { { p_startIndent, res } };
    }

    render_t vh_line_to_operation::render_op( u64 p_startIndent ) const {
        std::string res{ };
        if( !_options.empty( ) ) {
            res += "[";
            res += _options.to_string( );
            res += "]";
        }
        if( !m_position.empty( ) ) {
            res += "|- (" + m_position.to_string( ) + ")";
        } else {
            res += "|- cycle";
        }

        return { { p_startIndent, res } };
    }

    render_t rectangle_operation::render_op( u64 p_startIndent ) const {
        std::string res{ };
        if( !_options.empty( ) ) {
            res += "[";
            res += _options.to_string( );
            res += "]";
        }
        if( !m_position.empty( ) ) { res += "rectangle (" + m_position.to_string( ) + ")"; }

        return { { p_startIndent, res } };
    }

    render_t coordinate_command::render( u64 p_startIndent, bool p_internal ) const {
        std::string result = "";

        if( !p_internal ) { result += "\\"; }
        result += "coordinate";

        if( !_options.empty( ) ) {
            result += "[";
            result += _options.to_string( );
            result += "]";
        }

        result += " (" + m_name + ")";
        if( !m_position.empty( ) ) { result += " at (" + m_position.to_string( ) + ")"; }

        if( !p_internal ) { result += ";"; }

        return { { p_startIndent, result } };
    }

    render_t node_command::render( u64 p_startIndent, bool p_internal ) const {
        std::string result = "";

        if( !p_internal ) { result += "\\"; }
        result += "node";

        if( !_options.empty( ) ) {
            result += "[";
            result += _options.to_string( );
            result += "]";
        }

        if( m_name != EMPTY_STR ) { result += " (" + m_name + ")"; }
        if( !m_position.empty( ) ) { result += " at (" + m_position.to_string( ) + ")"; }
        result += " {" + m_content + "}";

        if( !p_internal ) { result += ";"; }

        return { { p_startIndent, result } };
    }

    std::set<std::string> scope_command::libraries( ) const {
        return _content.libraries( );
    }

    std::set<std::string> scope_command::packages( ) const {
        return _content.packages( );
    }

    render_t scope_command::render( u64 p_startIndent ) const {
        return _content.render( p_startIndent, "scope" );
    }

} // namespace TIKZ
