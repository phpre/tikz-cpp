#include <deque>
#include <string>

#include "tikz_circuit.h"
#include "tikz_default_paths.h"
#include "tikz_util.h"

namespace TIKZ {
    double GATE_HEIGHT        = .5;
    double GATE_WIDTH         = .5;
    double DEFAULT_GATE_SEP_X = 2 * GATE_WIDTH;
    double DEFAULT_GATE_SEP_Y = 1.75 * GATE_HEIGHT;

    std::map<std::string, tikz_point> place_input_gate( picture& p_pic, const std::string& p_text,
                                                        tikz_point p_topLeft, u64 p_fanOut,
                                                        const std::string& p_name, color p_color,
                                                        color           p_bgColor,
                                                        const kv_store& p_options ) {
        auto cy = p_topLeft.m_y - GATE_HEIGHT / 2;
        auto cx = p_topLeft.m_x + GATE_WIDTH / 2;

        // place empty circle + name below
        p_pic.place_node( tikz_point( cx, cy ), EMPTY_STR,
                          OPT::DRAW( p_bgColor ) | OPT::FILL( p_bgColor ) | OPT::LW_SUPPORT_LINE
                              | OPT::CIRCLE | OPT::DOUBLE( p_color ) | OPT::INNER_SEP( "2.5pt" )
                              | OPT::OUTER_SEP( "0pt" ) );

        std::map<std::string, tikz_point> res{ };
        auto smno = OPT::FILL( p_bgColor ) | OPT::CIRCLE | OPT::INNER_SEP( "1.25pt" )
                    | OPT::OUTER_SEP( "1.25pt" ) | p_options;
        for( u64 i = 1; i <= p_fanOut; ++i ) {
            double     py   = cy - GATE_HEIGHT / 4.0 * ( i - 1 - ( p_fanOut - 1 ) / 2.0 );
            auto       name = std::format( "{}_out_{}", p_name, i );
            tikz_point pos{ p_topLeft.m_x + GATE_WIDTH, py };

            p_pic.place_node( pos, EMPTY_STR, smno, name );
            place_double_arrow( p_pic, pos, pos + tikz_point{ .1, 0.0 }, p_color, p_bgColor,
                                p_options );
            res[ name ] = pos;
        }

        p_topLeft = p_topLeft + tikz_point{ 0.0, -7.0 * GATE_HEIGHT / 8 };
        p_pic.add_command(
            std::make_shared<math_command>( width_macro( 1.75 * GATE_WIDTH, p_text ) ) );
        p_pic.place_text( textsize_tiny( VSIZE_CORRECTION + p_text ),
                          p_topLeft + tikz_point{ GATE_WIDTH / 2.0, 0.01 },
                          OPT::ANCHOR_NORTH | OPT::INNER_SEP( "0.75pt" )
                              | OPT::TEXT_COLOR( p_color ) | XSCALE_TO_WIDTH
                              | OPT::TRANSFORM_SHAPE );

        return res;
    }

    std::map<std::string, tikz_point> place_output_gate( picture& p_pic, const std::string& p_text,
                                                         tikz_point         p_topLeft,
                                                         const std::string& p_name, color p_color,
                                                         color           p_bgColor,
                                                         const kv_store& p_options ) {
        p_topLeft = p_topLeft + tikz_point{ 0.0, -GATE_HEIGHT / 2 };
        std::map<std::string, tikz_point> res{ };
        auto smno = OPT::FILL( p_bgColor ) | OPT::CIRCLE | OPT::INNER_SEP( "1.25pt" )
                    | OPT::OUTER_SEP( ".125pt" ) | p_options;
        auto name = std::format( "{}_in_1", p_name );

        p_pic.place_node( p_topLeft, EMPTY_STR, smno, name );
        place_double_arrow( p_pic, p_topLeft, p_topLeft + tikz_point{ .1, 0.0 }, p_color, p_bgColor,
                            p_options );
        res[ name ] = p_topLeft;

        // place empty circle + name below
        p_pic.place_node( p_topLeft + tikz_point( GATE_WIDTH / 2, 0.0 ), EMPTY_STR,
                          OPT::DRAW( p_bgColor ) | OPT::FILL( p_bgColor ) | OPT::LW_SUPPORT_LINE
                              | OPT::CIRCLE | OPT::DOUBLE( p_color ) | OPT::INNER_SEP( "2.5pt" )
                              | OPT::OUTER_SEP( "0pt" ) );

        p_topLeft = p_topLeft + tikz_point{ 0.0, -GATE_HEIGHT / 4 - GATE_HEIGHT / 8 };
        p_pic.add_command(
            std::make_shared<math_command>( width_macro( 1.5 * GATE_WIDTH, p_text ) ) );
        p_pic.place_text( textsize_tiny( VSIZE_CORRECTION + p_text ),
                          p_topLeft + tikz_point{ GATE_WIDTH / 2.0, 0.01 },
                          OPT::ANCHOR_NORTH | OPT::INNER_SEP( "0.75pt" )
                              | OPT::TEXT_COLOR( p_color ) | XSCALE_TO_WIDTH
                              | OPT::TRANSFORM_SHAPE );

        return res;
    }

    std::map<std::string, tikz_point> place_const_gate( picture& p_pic, const std::string& p_text,
                                                        tikz_point p_topLeft, u64 p_fanOut,
                                                        const std::string& p_name, color p_color,
                                                        color           p_bgColor,
                                                        const kv_store& p_options ) {
        // place text on top of line, add dot for each output
        auto width = ( p_fanOut + 1 ) * GATE_WIDTH / 1.25;

        auto smno = OPT::FILL( p_bgColor ) | OPT::CIRCLE | OPT::INNER_SEP( "1.25pt" ) | p_options;
        for( u64 i = 1; i <= p_fanOut; ++i ) {
            double     px   = width * i * .5 / ( p_fanOut + 1 );
            auto       name = std::format( "{}_out_{}", p_name, i );
            tikz_point pos  = p_topLeft + tikz_point{ px, -.02 };
            p_pic.place_node( pos, EMPTY_STR, smno, name );
        }

        p_pic.place_line( p_topLeft, p_topLeft + tikz_point{ width, 0.0 },
                          OPT::DRAW( p_bgColor ) | OPT::DOUBLE( p_color ) | OPT::LW_THIN_OUTLINE
                              | OPT::LINE_CAP_ROUND | OPT::DOUBLE_DISTANCE( "1pt" )
                              | OPT::ROUNDED_CORNERS( "1pt" ) | p_options );

        std::map<std::string, tikz_point> res{ };
        // add anchors for each in/out
        smno = OPT::FILL( p_color ) | OPT::CIRCLE | OPT::INNER_SEP( "0.75pt" )
               | OPT::OUTER_SEP( ".5pt" ) | p_options;
        for( u64 i = 1; i <= p_fanOut; ++i ) {
            double     px   = width * i * .5 / ( p_fanOut + 1 );
            auto       name = std::format( "{}_out_{}", p_name, i );
            tikz_point pos  = p_topLeft + tikz_point{ px, -.02 };
            p_pic.place_node( pos, EMPTY_STR, smno, name );
            res[ name ] = pos;
        }

        p_pic.add_command( std::make_shared<math_command>( width_macro( 2 * width, p_text ) ) );
        p_pic.place_text(
            textsize_tiny( VSIZE_CORRECTION + p_text ), p_topLeft + tikz_point{ width / 2.0, 0.01 },
            OPT::ANCHOR_SOUTH | OPT::INNER_SEP( "0.75pt" ) | OPT::TEXT_COLOR( p_color )
                | XSCALE_TO_WIDTH | OPT::TRANSFORM_SHAPE );

        return res;
    }

    std::map<std::string, tikz_point> place_op_gate( picture& p_pic, const std::string& p_text,
                                                     tikz_point p_topLeft, u64 p_fanIn,
                                                     u64 p_fanOut, const std::string& p_name,
                                                     color p_color, color p_bgColor, bool p_invert,
                                                     const kv_store& p_options ) {
        auto numio = std::max( p_fanIn, p_fanOut ) + 1.0;
        if( numio < 3 ) { numio = 3; }
        auto height = ( numio - 1 ) * GATE_HEIGHT / 2.0;

        auto smno = OPT::FILL( p_bgColor ) | OPT::CIRCLE | OPT::INNER_SEP( "1.25pt" ) | p_options;
        for( u64 i = 1; i <= p_fanIn; ++i ) {
            double py = p_topLeft.m_y - height * ( i - .5 ) / ( p_fanIn );
            p_pic.place_node( tikz_point{ p_topLeft.m_x - .02, py }, EMPTY_STR, smno );
        }
        for( u64 i = 1; i <= p_fanOut; ++i ) {
            double py = p_topLeft.m_y - height * ( i - .5 ) / ( p_fanOut );
            p_pic.place_node( tikz_point{ p_topLeft.m_x + GATE_WIDTH + .02, py }, EMPTY_STR, smno );
        }

        // place rectangle
        p_pic.place_rectangle( p_topLeft, p_topLeft + tikz_point{ GATE_WIDTH, -height },
                               OPT::DRAW( p_bgColor ) | OPT::FILL( p_invert ? p_color : p_bgColor )
                                   | OPT::DOUBLE( p_color ) | OPT::LW_THIN_OUTLINE
                                   | OPT::DOUBLE_DISTANCE( "1pt" ) | OPT::ROUNDED_CORNERS( "1pt" )
                                   | p_options );

        // place text inside rectangle
        p_pic.add_command( std::make_shared<math_command>( width_macro( GATE_WIDTH, p_text ) ) );
        p_pic.place_text(
            text_boldface( p_text ), p_topLeft + tikz_point{ GATE_WIDTH / 2.0, -height / 2.0 },
            OPT::INNER_SEP( "0pt" ) | OPT::TEXT_COLOR( p_invert ? p_bgColor : p_color )
                | XSCALE_TO_WIDTH | OPT::TRANSFORM_SHAPE );

        std::map<std::string, tikz_point> res{ };

        // add anchors for each in/out
        smno = OPT::FILL( p_color ) | OPT::CIRCLE | OPT::INNER_SEP( "0.75pt" )
               | OPT::OUTER_SEP( ".5pt" ) | p_options;
        for( u64 i = 1; i <= p_fanIn; ++i ) {
            double     py   = p_topLeft.m_y - height * ( i - .5 ) / ( p_fanIn );
            auto       name = std::format( "{}_in_{}", p_name, i );
            tikz_point pos{ p_topLeft.m_x - .02, py };
            p_pic.place_node( pos, EMPTY_STR, smno, name );
            res[ name ] = pos;
        }
        for( u64 i = 1; i <= p_fanOut; ++i ) {
            double     py   = p_topLeft.m_y - height * ( i - .5 ) / ( p_fanOut );
            auto       name = std::format( "{}_out_{}", p_name, i );
            tikz_point pos{ p_topLeft.m_x + GATE_WIDTH + .02, py };
            p_pic.place_node( pos, EMPTY_STR, smno, name );
            res[ name ] = pos;
        }
        return res;
    }

    void place_const_connection( picture& p_pic, const std::string& p_startName,
                                 const std::string& p_endName ) {
        p_pic.place_vh_line( tikz_position{ p_startName }, tikz_position{ p_endName },
                             OPT::DRAW( COLOR_TEXT.to_bg( ) ) | OPT::DOUBLE( COLOR_TEXT )
                                 | OPT::ROUNDED_CORNERS( "1pt" ) | OPT::LW_VERY_THIN_OUTLINE
                                 | OPT::DOUBLE_DISTANCE( ".75pt" ) );
    }

    void place_gate_connection( picture& p_pic, const std::string& p_startName, tikz_point p_start,
                                const std::string& p_endName, tikz_point p_end ) {
        if( p_start.m_y == p_end.m_y ) {
            p_pic.place_line( tikz_position{ p_startName }, tikz_position{ p_endName },
                              OPT::DRAW( COLOR_TEXT.to_bg( ) ) | OPT::DOUBLE( COLOR_TEXT )
                                  | OPT::ROUNDED_CORNERS( "1pt" ) | OPT::LW_VERY_THIN_OUTLINE
                                  | OPT::DOUBLE_DISTANCE( ".75pt" ) );
        } else {
            p_pic.place_hvh_line( tikz_position{ p_startName }, ( p_start + p_end ) * .5,
                                  tikz_position{ p_endName },
                                  OPT::DRAW( COLOR_TEXT.to_bg( ) ) | OPT::DOUBLE( COLOR_TEXT )
                                      | OPT::ROUNDED_CORNERS( "1pt" ) | OPT::LW_VERY_THIN_OUTLINE
                                      | OPT::DOUBLE_DISTANCE( ".75pt" ) );
        }
    }
} // namespace TIKZ
