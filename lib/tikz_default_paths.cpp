#include "tikz_command.h"
#include "tikz_default_paths.h"

namespace TIKZ {
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
