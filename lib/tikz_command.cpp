#include "tikz_command.h"

namespace TIKZ {
    render_t path_command::render( u64 p_time, u64 p_startIndent ) const {
        if( p_time && !m_times.count( p_time ) ) { return { }; }
        if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

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
            result.append_range( op->render_op( p_time, 1 + p_startIndent ) );
        }

        result.push_back( { p_startIndent, ";" } );
        return result;
    }

    render_t move_to_operation::render_op( u64 p_time, u64 p_startIndent ) const {
        if( p_time && !m_times.count( p_time ) ) { return { }; }
        if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

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

    render_t line_to_operation::render_op( u64 p_time, u64 p_startIndent ) const {
        if( p_time && !m_times.count( p_time ) ) { return { }; }
        if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

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

    render_t rectangle_operation::render_op( u64 p_time, u64 p_startIndent ) const {
        if( p_time && !m_times.count( p_time ) ) { return { }; }
        if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

        std::string res{ };
        if( !_options.empty( ) ) {
            res += "[";
            res += _options.to_string( );
            res += "]";
        }
        if( !m_position.empty( ) ) { res += "rectangle (" + m_position.to_string( ) + ")"; }

        return { { p_startIndent, res } };
    }

    render_t coordinate_command::render( u64 p_time, u64 p_startIndent, bool p_internal ) const {
        if( p_time && !m_times.count( p_time ) ) { return { }; }
        if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

        std::string result = "";

        if( !p_internal ) { result += "\\"; }
        result += "coordinate";

        if( !_options.empty( ) ) {
            result += "[";
            result += _options.to_string( );
            result += "]";
        }

        if( m_name != EMPTY_STR ) { result += " (" + m_name + ")"; }
        if( !m_position.empty( ) ) { result += " at (" + m_position.to_string( ) + ")"; }

        if( !p_internal ) { result += ";"; }

        return { { p_startIndent, result } };
    }

    render_t node_command::render( u64 p_time, u64 p_startIndent, bool p_internal ) const {
        if( p_time && !m_times.count( p_time ) ) { return { }; }
        if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

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
} // namespace TIKZ
