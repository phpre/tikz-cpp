#include "tikz_picture.h"

namespace TIKZ {
    picture::picture( kv_store p_options )
        : _packages{ }, _libraries{ }, _options{ p_options }, _content{ } {
    }

    render_t picture::render( u64 p_time, u64 p_startIndent ) const {
        render_t result{ };

        if( _content.empty( ) ) {
            // picture is empty
            return result;
        }

        for( const auto& cmd : _content ) {
            result.append_range( cmd->render( p_time, p_startIndent + 1 ) );
        }

        std::string env = "\\begin{tikzpicture}";
        if( !_options.empty( ) ) {
            env += "[";
            env += _options.to_string( );
            env += "]";
        }

        result.push_front( { p_startIndent, env } );
        result.push_back( { p_startIndent, "\\end{tikzpicture}" } );

        return result;
    }

    void picture::add_package( const std::string& p_package ) {
        _packages.insert( p_package );
    }

    void picture::add_library( const std::string& p_library ) {
        _libraries.insert( p_library );
    }

    void picture::add_command( std::shared_ptr<command> p_command ) {
        if( !p_command ) { return; }
        _content.emplace_back( p_command );
    }

    void picture::place_coordinate( tikz_position p_position, const std::string& p_name,
                                    const kv_store& p_options ) {
        _content.emplace_back(
            std::make_shared<coordinate_command>( p_position, p_name, p_options ) );
    }

    void picture::place_node( tikz_position p_position, const std::string& p_content,
                              const kv_store& p_options, const std::string& p_name ) {
        _content.emplace_back(
            std::make_shared<node_command>( p_position, p_content, p_options, p_name ) );
    }

    void picture::place_line( tikz_position p_topLeft, tikz_position p_bottomRight,
                              const kv_store& p_options ) {
        std::deque<std::shared_ptr<path_operation>> line{
            std::make_shared<line_to_operation>( p_bottomRight ) };
        _content.emplace_back( std::make_shared<path_command>( p_topLeft, line, p_options ) );
    }

    void picture::place_rectangle( tikz_position p_topLeft, tikz_position p_bottomRight,
                                   const kv_store& p_options ) {
        std::deque<std::shared_ptr<path_operation>> rect{
            std::make_shared<rectangle_operation>( p_bottomRight ) };
        _content.emplace_back( std::make_shared<path_command>( p_topLeft, rect, p_options ) );
    }

    void picture::place_simple_path( std::deque<tikz_position> p_coords,
                                     const kv_store&           p_options ) {
        if( p_coords.size( ) < 2 ) { return; }

        std::deque<std::shared_ptr<path_operation>> path{ };
        for( u64 i = 1; i < p_coords.size( ); ++i ) {
            path.emplace_back( std::make_shared<line_to_operation>( p_coords[ i ] ) );
        }
        _content.emplace_back(
            std::make_shared<path_command>( p_coords.front( ), path, p_options ) );
    }

    void picture::place_cross( tikz_position p_position, const kv_store& p_options,
                               double p_lineWidth, double p_size, const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 50 ];
        snprintf( buf1, 29, "%5.3lfpt", p_lineWidth );
        snprintf( buf2, 49, "2 * (%5.3lfpt - \\pgflinewidth)", p_size );

        place_node( p_position, EMPTY_STR,
                    OPT::CROSS_OUT | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                        | OPT::LINE_WIDTH( std::string{ buf1 } ) | OPT::INNER_SEP( "0pt" )
                        | OPT::OUTER_SEP( "0pt" ) | p_options,
                    p_name );
    }

    void picture::place_circle( tikz_position p_position, const kv_store& p_options,
                                double p_lineWidth, double p_size, const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 50 ];
        snprintf( buf1, 29, "%5.3lfpt", p_lineWidth );
        snprintf( buf2, 49, "2 * (%5.3lfpt - \\pgflinewidth)", p_size );

        place_node( p_position, EMPTY_STR,
                    OPT::CIRCLE | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                        | OPT::LINE_WIDTH( std::string{ buf1 } ) | OPT::INNER_SEP( "0pt" )
                        | OPT::OUTER_SEP( "0pt" ) | p_options,
                    p_name );
    }

    void picture::place_double_cross( tikz_position p_position, const kv_store& p_options,
                                      double p_lineWidth, double p_outlineWidth, double p_size,
                                      const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 30 ], buf3[ 30 ];
        snprintf( buf1, 29, "%5.3lfpt", p_outlineWidth );
        snprintf( buf3, 29, "%5.3lfpt", p_lineWidth - 2 * p_outlineWidth );
        snprintf( buf2, 29, "%5.3lfpt", 2 * ( p_size - p_lineWidth ) - 2 * p_outlineWidth );

        place_node( p_position, EMPTY_STR,
                    OPT::CROSS_OUT | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                        | OPT::LINE_WIDTH( std::string{ buf1 } )
                        | OPT::DOUBLE_DISTANCE( std::string{ buf3 } ) | OPT::INNER_SEP( "0pt" )
                        | OPT::OUTER_SEP( "0pt" ) | OPT::LINE_CAP_RECT | p_options,
                    p_name );
    }

    void picture::place_double_circle( tikz_position p_position, const kv_store& p_options,
                                       double p_lineWidth, double p_outlineWidth, double p_size,
                                       const std::string& p_name ) {
        char buf1[ 30 ], buf2[ 30 ], buf3[ 30 ];
        snprintf( buf1, 29, "%5.3lfpt", p_outlineWidth );
        snprintf( buf3, 29, "%5.3lfpt", p_lineWidth - 2 * p_outlineWidth );
        snprintf( buf2, 29, "%5.3lfpt", 2 * ( p_size - p_lineWidth ) );

        place_node( p_position, EMPTY_STR,
                    OPT::CIRCLE | OPT::DRAW | OPT::MINIMUM_SIZE( std::string{ buf2 } )
                        | OPT::LINE_WIDTH( std::string{ buf1 } )
                        | OPT::DOUBLE_DISTANCE( std::string{ buf3 } ) | OPT::INNER_SEP( "0pt" )
                        | OPT::OUTER_SEP( "0pt" ) | p_options,
                    p_name );
    }
} // namespace TIKZ
