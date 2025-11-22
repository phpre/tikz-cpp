#include "tikz_command.h"

namespace TIKZ {
    const std::string CROSS_STYLE
        = "\\tikzset{cross/.style={cross out, draw, minimum size=2*(#1-\\pgflinewidth), inner sep=0pt, outer sep=0pt}}\n";

    node_command node_command::place_text( const std::string& p_text, tikz_position p_position,
                                           const kv_store& p_options, const std::string& p_name ) {
        node_command res{ };
        res._options   = p_options;
        res.m_position = p_position;
        res.m_name     = p_name;
        res.m_content  = p_text;

        return res;
    }

    node_command node_command::place_cross( tikz_position p_position, const kv_store& p_options,
                                            double p_lineWidth, double p_size,
                                            const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 50 ];
        snprintf( buf1, 29, "%5.3lfpt", p_lineWidth );
        snprintf( buf2, 49, "2 * (%5.3lfpt - \\pgflinewidth)", p_size );

        node_command res{ };
        res._options = OPT::CROSS_OUT | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                       | OPT::LINE_WIDTH( std::string{ buf1 } ) | OPT::INNER_SEP( "0pt" )
                       | OPT::OUTER_SEP( "0pt" ) | p_options;
        res.m_position = p_position;

        res.m_name    = p_name;
        res.m_content = EMPTY_STR;
        return res;
    }

    node_command node_command::place_circle( tikz_position p_position, const kv_store& p_options,
                                             double p_lineWidth, double p_size,
                                             const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 50 ];
        snprintf( buf1, 29, "%5.3lfpt", p_lineWidth );
        snprintf( buf2, 49, "2 * (%5.3lfpt - \\pgflinewidth)", p_size );

        node_command res{ };
        res._options = OPT::CIRCLE | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                       | OPT::LINE_WIDTH( std::string{ buf1 } ) | OPT::INNER_SEP( "0pt" )
                       | OPT::OUTER_SEP( "0pt" ) | p_options;
        res.m_position = p_position;

        res.m_name    = p_name;
        res.m_content = EMPTY_STR;
        return res;
    }

    node_command node_command::place_double_cross( tikz_position   p_position,
                                                   const kv_store& p_options, double p_lineWidth,
                                                   double p_outlineWidth, double p_size,
                                                   const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 30 ], buf3[ 30 ];
        snprintf( buf1, 29, "%5.3lfpt", p_outlineWidth );
        snprintf( buf3, 29, "%5.3lfpt", p_lineWidth - 2 * p_outlineWidth );
        snprintf( buf2, 29, "%5.3lfpt", 2 * ( p_size - p_lineWidth ) - 2 * p_outlineWidth );

        node_command res{ };
        res._options = OPT::CROSS_OUT | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                       | OPT::LINE_WIDTH( std::string{ buf1 } )
                       | OPT::DOUBLE_DISTANCE( std::string{ buf3 } ) | OPT::INNER_SEP( "0pt" )
                       | OPT::OUTER_SEP( "0pt" ) | OPT::LINE_CAP_RECT | p_options;
        res.m_position = p_position;

        res.m_name    = p_name;
        res.m_content = EMPTY_STR;
        return res;
    }

    node_command node_command::place_double_circle( tikz_position   p_position,
                                                    const kv_store& p_options, double p_lineWidth,
                                                    double p_outlineWidth, double p_size,
                                                    const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 30 ], buf3[ 30 ];
        snprintf( buf1, 29, "%5.3lfpt", p_outlineWidth );
        snprintf( buf3, 29, "%5.3lfpt", p_lineWidth - 2 * p_outlineWidth );
        snprintf( buf2, 29, "%5.3lfpt", 2 * ( p_size - p_lineWidth ) );

        node_command res{ };
        res._options = OPT::CIRCLE | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                       | OPT::LINE_WIDTH( std::string{ buf1 } )
                       | OPT::DOUBLE_DISTANCE( std::string{ buf3 } ) | OPT::INNER_SEP( "0pt" )
                       | OPT::OUTER_SEP( "0pt" ) | p_options;
        res.m_position = p_position;

        res.m_name    = p_name;
        res.m_content = EMPTY_STR;
        return res;
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
        result += " at (" + m_position.to_string( ) + ")";
        result += " {" + m_content + "}";

        if( !p_internal ) { result += ";"; }

        return { { p_startIndent, result } };
    }
} // namespace TIKZ
