#include "tikz_command.h"

namespace TIKZ {
    node_command node_command::place_text( const std::string& p_text, tikz_position p_position,
                                           const std::string& p_name, const kv_store& p_options ) {
        node_command res{ };
        res.m_options  = p_options;
        res.m_position = p_position;
        res.m_name     = p_name;
        res.m_content  = p_text;

        return res;
    }

    render_t node_command::render( u32 p_time, u32 p_startIndent, bool p_internal ) const {
        if( p_time && !m_times.count( p_time ) ) { return { }; }
        if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

        std::string result = "";

        if( !p_internal ) { result += "\\"; }
        result += "node";

        if( !m_options.empty( ) ) {
            result += "[";
            result += kv_to_string( m_options );
            result += "] ";
        }

        if( m_name != EMPTY_STR ) { result += "(" + m_name + ") "; }
        result += "at (" + m_position.to_string( ) + ") ";
        result += "{" + m_content + "}";

        if( !p_internal ) { result += ";"; }

        return { { p_startIndent, result } };
    }
} // namespace TIKZ
