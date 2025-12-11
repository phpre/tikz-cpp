#include "tikz_command.h"
#include "tikz_default_paths.h"

namespace TIKZ {
    void place_selected_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                               color p_lineColor, color p_fillColor, double p_angle,
                               const kv_store& p_options ) {
        auto opt = OPT::ROTATE( std::format( "{:5.3f}", p_angle ) ) | OPT::DRAW( p_fillColor )
                   | OPT::DOUBLE( p_lineColor ) | OPT::DOUBLE_DISTANCE( ".75pt" )
                   | OPT::FILL( p_fillColor ) | OPT::ROUNDED_CORNERS( ".2pt" ) | p_options;
        p_pic.add_library( "calc" );

        std::deque<std::shared_ptr<path_operation>> path{ };
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, 3.5pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, 1.25pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_startPos.to_string( ) + ") + (2.75pt, 1.25pt)$" ) );

        path.emplace_back( std::make_shared<arc_to_operation>( 25.0, 333.5, 3.0 ) );

        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_startPos.to_string( ) + ") + (2.75pt, -1.25pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, -1.25pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, -3.5pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>( tikz_position{ } ) );

        p_pic.add_command( std::make_shared<path_command>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-.5pt, 0pt)$", path, opt ) );
    }

    void place_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                      const kv_store& p_options, const kv_store& p_basicOptions ) {
        p_pic.place_line( p_startPos, p_endPos, p_basicOptions | p_options );
    }

    void place_cross( picture& p_pic, tikz_position p_position, const kv_store& p_options,
                      double p_lineWidth, double p_size, const std::string& p_name ) {
        p_pic.place_node(
            p_position, EMPTY_STR,
            OPT::CROSS_OUT | OPT::DRAW
                | OPT::MINIMUM_SIZE( std::format( "2 * ({:5.3f}pt - \\pgflinewidth)", p_size ) )
                | OPT::LINE_WIDTH( std::format( "{:5.3f}pt", p_lineWidth ) )
                | OPT::INNER_SEP( "0pt" ) | OPT::OUTER_SEP( "0pt" ) | p_options,
            p_name );
    }

    void place_circle( picture& p_pic, tikz_position p_position, const kv_store& p_options,
                       double p_lineWidth, double p_size, const std::string& p_name ) {
        p_pic.place_node(
            p_position, EMPTY_STR,
            OPT::CIRCLE | OPT::DRAW
                | OPT::MINIMUM_SIZE( std::format( "2 * ({:5.3f}pt - \\pgflinewidth)", p_size ) )
                | OPT::LINE_WIDTH( std::format( "{:5.3f}pt", p_lineWidth ) )
                | OPT::INNER_SEP( "0pt" ) | OPT::OUTER_SEP( "0pt" ) | p_options,
            p_name );
    }

    void place_double_cross( picture& p_pic, tikz_position p_position, const kv_store& p_options,
                             double p_lineWidth, double p_outlineWidth, double p_size,
                             const std::string& p_name ) {
        p_pic.place_node( p_position, EMPTY_STR,
                          OPT::CROSS_OUT | OPT::DRAW
                              | OPT::MINIMUM_SIZE( std::format(
                                  "{:5.3f}pt", 2 * ( p_size - p_lineWidth ) - 2 * p_outlineWidth ) )
                              | OPT::LINE_WIDTH( std::format( "{:5.3f}pt", p_outlineWidth ) )
                              | OPT::DOUBLE_DISTANCE(
                                  std::format( "{:5.3f}pt", p_lineWidth - 2 * p_outlineWidth ) )
                              | OPT::INNER_SEP( "0pt" ) | OPT::OUTER_SEP( "0pt" )
                              | OPT::LINE_CAP_RECT | p_options,
                          p_name );
    }

    void place_double_circle( picture& p_pic, tikz_position p_position, const kv_store& p_options,
                              double p_lineWidth, double p_outlineWidth, double p_size,
                              const std::string& p_name ) {
        p_pic.place_node(
            p_position, EMPTY_STR,
            OPT::CIRCLE | OPT::DRAW
                | OPT::MINIMUM_SIZE( std::format( "{:5.3f}pt", 2 * ( p_size - p_lineWidth ) ) )
                | OPT::LINE_WIDTH( std::format( "{:5.3f}pt", p_outlineWidth ) )
                | OPT::DOUBLE_DISTANCE(
                    std::format( "{:5.3f}pt", p_lineWidth - 2 * p_outlineWidth ) )
                | OPT::INNER_SEP( "0pt" ) | OPT::OUTER_SEP( "0pt" ) | p_options,
            p_name );
    }

    void place_puzzle_piece( picture& p_pic, tikz_point p_center, const kv_store& p_extraOptions,
                             puzzle_bend_t p_top, puzzle_bend_t p_left, puzzle_bend_t p_bottom,
                             puzzle_bend_t p_right, double p_cornerBend, double p_innerBend,
                             double p_outerBend, double p_coreSize, double p_totalSize,
                             double p_bendpointSize, const kv_store& p_options ) {
        double rad    = p_coreSize / 2;
        double outrad = p_totalSize / 2;
        double midrad = p_bendpointSize / 2;

        double s1 = rad * .15, s2 = rad * .41, s3 = rad * .2;
        double s4 = rad * .25, s5 = rad * .47, s6 = rad * .78;

        std::deque<std::shared_ptr<path_operation>> path{ };
        if( p_left == puzzle_bend_t::CURVED_OUT ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };

            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ rad, s1 },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_outerBend ) ) ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ midrad, s2 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ outrad, s3 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ outrad, -s3 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ midrad, -s2 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ rad, -s1 } ) );

            path.push_back( std::make_shared<scope_operation>( bend ) );
        } else if( p_left == puzzle_bend_t::CURVED_IN ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };
            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ rad, s4 },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_cornerBend ) ) ) );
            {
                std::deque<std::shared_ptr<path_operation>> ibend{ };
                ibend.push_back( std::make_shared<line_to_operation>(
                    p_center + tikz_point{ s6, s5 },
                    OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_innerBend ) ) ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ s2, s4 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ s2, -s4 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ s6, -s5 } ) );
                bend.push_back( std::make_shared<scope_operation>( ibend ) );
            }
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ rad, -s4 } ) );
            path.push_back( std::make_shared<scope_operation>( bend ) );
        }

        path.push_back( std::make_shared<line_to_operation>( p_center + tikz_point{ rad, -rad } ) );

        if( p_top == puzzle_bend_t::CURVED_OUT ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };

            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ s1, -rad },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_outerBend ) ) ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ s2, -midrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ s3, -outrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -s3, -outrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -s2, -midrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -s1, -rad } ) );

            path.push_back( std::make_shared<scope_operation>( bend ) );
        } else if( p_top == puzzle_bend_t::CURVED_IN ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };
            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ s4, -rad },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_cornerBend ) ) ) );
            {
                std::deque<std::shared_ptr<path_operation>> ibend{ };
                ibend.push_back( std::make_shared<line_to_operation>(
                    p_center + tikz_point{ s5, -s6 },
                    OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_innerBend ) ) ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ s4, -s2 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ -s4, -s2 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ -s5, -s6 } ) );
                bend.push_back( std::make_shared<scope_operation>( ibend ) );
            }
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -s4, -rad } ) );
            path.push_back( std::make_shared<scope_operation>( bend ) );
        }

        path.push_back(
            std::make_shared<line_to_operation>( p_center + tikz_point{ -rad, -rad } ) );

        if( p_right == puzzle_bend_t::CURVED_OUT ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };

            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ -rad, -s1 },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_outerBend ) ) ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -midrad, -s2 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -outrad, -s3 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -outrad, s3 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -midrad, s2 } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -rad, s1 } ) );

            path.push_back( std::make_shared<scope_operation>( bend ) );
        } else if( p_right == puzzle_bend_t::CURVED_IN ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };
            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ -rad, -s4 },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_cornerBend ) ) ) );
            {
                std::deque<std::shared_ptr<path_operation>> ibend{ };
                ibend.push_back( std::make_shared<line_to_operation>(
                    p_center + tikz_point{ -s6, -s5 },
                    OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_innerBend ) ) ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ -s2, -s4 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ -s2, s4 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ -s6, s5 } ) );
                bend.push_back( std::make_shared<scope_operation>( ibend ) );
            }
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -rad, s4 } ) );
            path.push_back( std::make_shared<scope_operation>( bend ) );
        }

        path.push_back( std::make_shared<line_to_operation>( p_center + tikz_point{ -rad, rad } ) );

        if( p_bottom == puzzle_bend_t::CURVED_OUT ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };

            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ -s1, rad },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_outerBend ) ) ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -s2, midrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ -s3, outrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ s3, outrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ s2, midrad } ) );
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ s1, rad } ) );

            path.push_back( std::make_shared<scope_operation>( bend ) );
        } else if( p_bottom == puzzle_bend_t::CURVED_IN ) {
            std::deque<std::shared_ptr<path_operation>> bend{ };
            bend.push_back( std::make_shared<line_to_operation>(
                p_center + tikz_point{ -s4, rad },
                OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_cornerBend ) ) ) );
            {
                std::deque<std::shared_ptr<path_operation>> ibend{ };
                ibend.push_back( std::make_shared<line_to_operation>(
                    p_center + tikz_point{ -s5, s6 },
                    OPT::ROUNDED_CORNERS( std::format( "{:5.3f}pt", p_innerBend ) ) ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ -s4, s2 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ s4, s2 } ) );
                ibend.push_back(
                    std::make_shared<line_to_operation>( p_center + tikz_point{ s5, s6 } ) );
                bend.push_back( std::make_shared<scope_operation>( ibend ) );
            }
            bend.push_back(
                std::make_shared<line_to_operation>( p_center + tikz_point{ s4, rad } ) );
            path.push_back( std::make_shared<scope_operation>( bend ) );
        }

        path.push_back( std::make_shared<line_to_operation>( tikz_position{ } ) );
        p_pic.add_command( std::make_shared<path_command>( ( p_center + tikz_point{ rad, rad } ),
                                                           path, p_options | p_extraOptions ) );
    }

} // namespace TIKZ
