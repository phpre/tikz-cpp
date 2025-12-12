#include "tikz_command.h"
#include "tikz_default_paths.h"
#include "tikz_option.h"
#include "tikz_picture.h"
#include "tikz_string.h"

namespace TIKZ {
    void place_alignment_graph_label( picture& p_pic, const stylized_string& p_P,
                                      const stylized_string& p_T, tikz_point p_gridTopLeft ) {
        place_string( p_pic, p_T, p_gridTopLeft + tikz_point{ .0, CHAR_HEIGHT + .25 } );
        place_string_vertical( p_pic, p_P, p_gridTopLeft + tikz_point{ -CHAR_WIDTH - .25, .0 } );
    }

    vertex_grid place_alignment_graph( picture& p_pic, const std::string& p_P, fragmentco p_fragP,
                                       const std::string& p_T, fragmentco p_fragT,
                                       tikz_point p_gridCellSize, tikz_point p_gridTopLeft,
                                       color p_color ) {
        vertex_grid vg{ p_gridTopLeft, p_gridCellSize };
        vg.place_vertices( p_pic, vertex::unselected_vertex( p_color ), p_fragT.length( ) + 1,
                           p_fragP.length( ) + 1, p_fragT.closed_begin( ),
                           p_fragP.closed_begin( ) );

        for( u64 x = p_fragT.closed_begin( ); x <= p_fragT.open_end( ); ++x ) {
            for( u64 y = p_fragP.closed_begin( ); y <= p_fragP.open_end( ); ++y ) {
                if( x > p_fragT.closed_begin( ) ) {
                    place_arrow( p_pic, vg.label_for_pos( x - 1, y ), vg.label_for_pos( x, y ),
                                 OPT::DRAW( INS_COL.deemphasize_strong( ) ) );
                }
                if( y > p_fragP.closed_begin( ) ) {
                    place_arrow( p_pic, vg.label_for_pos( x, y - 1 ), vg.label_for_pos( x, y ),
                                 OPT::DRAW( DEL_COL.deemphasize_strong( ) ) );
                }
                if( x > p_fragT.closed_begin( ) && y > p_fragP.closed_begin( ) ) {
                    if( p_P[ y - 1 ] == p_T[ x - 1 ] ) {
                        place_arrow(
                            p_pic, vg.label_for_pos( x - 1, y - 1 ), vg.label_for_pos( x, y ),
                            OPT::LW_THICK_SUPPORT_LINE | OPT::DRAW( MAT_COL.deemphasize( ) ) );
                    } else {
                        place_arrow( p_pic, vg.label_for_pos( x - 1, y - 1 ),
                                     vg.label_for_pos( x, y ),
                                     OPT::DRAW( SUB_COL.deemphasize_strong( ) ) );
                    }
                }
            }
        }
        return vg;
    }

    vertex_grid place_alignment_graph_simple( picture& p_pic, fragmentco p_fragP,
                                              fragmentco p_fragT, tikz_point p_gridCellSize,
                                              tikz_point p_gridTopLeft, color p_color ) {
        vertex_grid vg{ p_gridTopLeft, p_gridCellSize };
        vg.place_vertices( p_pic, vertex::unselected_vertex( p_color ), p_fragT.length( ) + 1,
                           p_fragP.length( ) + 1, p_fragT.closed_begin( ),
                           p_fragP.closed_begin( ) );

        for( u64 x = p_fragT.closed_begin( ); x <= p_fragT.open_end( ); ++x ) {
            for( u64 y = p_fragP.closed_begin( ); y <= p_fragP.open_end( ); ++y ) {
                if( x > p_fragT.closed_begin( ) ) {
                    place_arrow( p_pic, vg.label_for_pos( x - 1, y ), vg.label_for_pos( x, y ),
                                 OPT::DRAW( p_color.deemphasize( ) ) );
                }
                if( y > p_fragP.closed_begin( ) ) {
                    place_arrow( p_pic, vg.label_for_pos( x, y - 1 ), vg.label_for_pos( x, y ),
                                 OPT::DRAW( p_color.deemphasize( ) ) );
                }
                if( x > p_fragT.closed_begin( ) && y > p_fragP.closed_begin( ) ) {
                    place_arrow( p_pic, vg.label_for_pos( x - 1, y - 1 ), vg.label_for_pos( x, y ),
                                 OPT::DRAW( p_color.deemphasize( ) ) );
                }
            }
        }
        return vg;
    }

    void place_alignment_on_coordinates( picture& p_pic, const vertex_grid& p_vg,
                                         const breakpoint_repn& p_brpnt, bool p_singleStep,
                                         bool p_showCost ) {
        for( u64 i = p_brpnt.size( ) - 1; i; --i ) {
            auto curpos  = p_brpnt[ i ].position( );
            auto prev    = p_brpnt[ i - 1 ];
            auto prevpos = prev.position( );

            // draw "selected arrow" from prev to curr.
            auto preva
                = point{ prevpos.first + ( !!prev.m_charP ), prevpos.second + ( !!prev.m_charT ) };

            if( preva != curpos ) {
                if( p_singleStep ) {
                    auto pp = point{ curpos.first - 1, curpos.second - 1 };
                    while( pp != preva ) {
                        place_selected_arrow( p_pic, p_vg.label_for_pos( pp.second, pp.first ),
                                              p_vg.label_for_pos( curpos.second, curpos.first ),
                                              MAT_COL, MAT_COL.to_bg( ), -45.0 );

                        if( p_showCost ) {
                            auto mp = ( p_vg.point_for_pos( pp.second, pp.first ) * .6 )
                                      + ( p_vg.point_for_pos( curpos.second, curpos.first ) * .4 );
                            place_edit_cost( p_pic, 0, mp, MAT_COL, MAT_COL.to_bg( ) );
                        }
                        curpos = pp;
                        pp     = point{ curpos.first - 1, curpos.second - 1 };
                    }
                }
                place_selected_arrow( p_pic, p_vg.label_for_pos( preva.second, preva.first ),
                                      p_vg.label_for_pos( curpos.second, curpos.first ), MAT_COL,
                                      MAT_COL.to_bg( ), -45.0 );
                if( p_showCost ) {
                    auto mp = ( p_vg.point_for_pos( preva.second, preva.first ) * .6 )
                              + ( p_vg.point_for_pos( curpos.second, curpos.first ) * .4 );
                    place_edit_cost( p_pic, 0, mp, MAT_COL, MAT_COL.to_bg( ) );
                }
            }

            if( prev.m_charT && prev.m_charP ) {
                // substitution
                place_selected_arrow( p_pic, p_vg.label_for_pos( prevpos.second, prevpos.first ),
                                      p_vg.label_for_pos( preva.second, preva.first ), SUB_COL,
                                      SUB_COL.to_bg( ), -45.0 );
                if( p_showCost ) {
                    auto mp = ( p_vg.point_for_pos( prevpos.second, prevpos.first ) * .6 )
                              + ( p_vg.point_for_pos( preva.second, preva.first ) * .4 );
                    place_edit_cost( p_pic, prev.m_cost, mp, SUB_COL, SUB_COL.to_bg( ) );
                }
            } else if( prev.m_charP ) {
                // deletion
                place_selected_arrow( p_pic, p_vg.label_for_pos( prevpos.second, prevpos.first ),
                                      p_vg.label_for_pos( preva.second, preva.first ), DEL_COL,
                                      DEL_COL.to_bg( ), -90.0 );
                if( p_showCost ) {
                    auto mp = ( p_vg.point_for_pos( prevpos.second, prevpos.first ) * .6 )
                              + ( p_vg.point_for_pos( preva.second, preva.first ) * .4 );
                    place_edit_cost( p_pic, prev.m_cost, mp, DEL_COL, DEL_COL.to_bg( ) );
                }
            } else if( prev.m_charT ) {
                // insertion
                place_selected_arrow( p_pic, p_vg.label_for_pos( prevpos.second, prevpos.first ),
                                      p_vg.label_for_pos( preva.second, preva.first ), INS_COL,
                                      INS_COL.to_bg( ), -0.0 );
                if( p_showCost ) {
                    auto mp = ( p_vg.point_for_pos( prevpos.second, prevpos.first ) * .6 )
                              + ( p_vg.point_for_pos( preva.second, preva.first ) * .4 );
                    place_edit_cost( p_pic, prev.m_cost, mp, INS_COL, INS_COL.to_bg( ) );
                }
            }
        }
    }

    void place_graph_slice_on_coordinates( picture& p_pic, const vertex_grid& p_vg,
                                           const graph_slice& p_gs, color p_color,
                                           color p_portalColor, color p_innerPortalColor,
                                           color p_topColor, color p_bottomColor ) {
        double vtxsz = 1.25;
        double inner = .5;
        if( p_color.is_non_empty( ) ) {
            vtxsz       = 1.75;
            inner       = .72;
            double glow = 0 * CHAR_GLOW;
            double corX = .5 * CHAR_GLOW;

            if( !p_gs.m_topFrontier.empty( ) || !p_gs.m_bottomFrontier.empty( ) ) {
                // print glow bubble

                auto opt = OPT::DRAW( p_color ) | OPT::FILL( p_color.to_bg( ) ) | OPT::LW_OUTLINE
                           | OPT::ROUNDED_CORNERS( "1.25pt" );
                std::deque<tikz_position> pts{ };

                // top line
                for( u64 i = 0; i < p_gs.m_topFrontier.size( ); ++i ) {
                    const auto& pt   = p_gs.m_topFrontier[ i ];
                    auto        tp   = p_vg.point_for_pos( pt.first, pt.second );
                    double      corx = corX;
                    if( !i ) { corx = -corX; }
                    if( p_gs.width( ) ) { corx = 0.0; }

                    double gy = glow;
                    if( p_topColor.is_non_empty( )
                        && pt.second == p_gs.m_topFrontier.front( ).second ) {
                        gy = 0.0;
                    }
                    pts.push_back( tikz_point{ tp.m_x + corx, tp.m_y + gy } );
                }

                // bottom line
                for( u64 i = 0; i < p_gs.m_bottomFrontier.size( ); ++i ) {
                    const auto& pt   = p_gs.m_bottomFrontier[ i ];
                    auto        tp   = p_vg.point_for_pos( pt.first, pt.second );
                    double      corx = -corX;
                    if( !i ) { corx = corX; }
                    if( p_gs.width( ) ) { corx = 0.0; }

                    double gy = glow;
                    if( p_bottomColor.is_non_empty( )
                        && pt.second == p_gs.m_bottomFrontier.front( ).second ) {
                        gy = 0.0;
                    }
                    pts.push_back( tikz_point{ tp.m_x + corx, tp.m_y - gy } );
                }
                pts.push_back( tikz_position{ } );
                p_pic.place_simple_path( pts, opt );
            }
        }

        if( p_portalColor.is_non_empty( ) ) {
            auto portal_inner = vertex::unselected_vertex( COLOR_FILL_WHITE, vtxsz - .7 );
            auto portal       = vertex::marked_vertex( p_portalColor, vtxsz, .75, vertex::ST_CIRCLE,
                                                       portal_inner );
            for( const auto& pt : p_gs.m_leftPortals ) {
                p_vg.place_vertex_on_coordinates( p_pic, portal, pt.first, pt.second );
            }
            for( const auto& pt : p_gs.m_rightPortals ) {
                p_vg.place_vertex_on_coordinates( p_pic, portal, pt.first, pt.second );
            }
        }

        if( p_innerPortalColor.is_non_empty( ) ) {
            auto portal_inner = vertex::unselected_vertex( COLOR_FILL_WHITE, vtxsz - .7 );
            auto portal = vertex::marked_vertex( p_innerPortalColor, vtxsz, .75, vertex::ST_CIRCLE,
                                                 portal_inner );
            for( const auto& pt : p_gs.m_innerPortals ) {
                p_vg.place_vertex_on_coordinates( p_pic, portal, pt.first, pt.second );
            }
        }

        if( p_topColor.is_non_empty( ) ) {
            auto portal_inner
                = vertex::unselected_vertex( COLOR_FILL_WHITE, inner, .75, vertex::ST_TRIANGLE_UP );
            auto portal
                = vertex::marked_vertex( p_topColor, vtxsz, .75, vertex::ST_CIRCLE, portal_inner );
            auto [ x, y ] = p_gs.m_topFrontier.front( );
            u64 xm        = x;
            for( const auto& pt : p_gs.m_topFrontier ) {
                if( pt.second == y ) {
                    xm = pt.first;
                } else {
                    break;
                }
            }
            for( auto i = x; i <= xm; ++i ) {
                p_vg.place_vertex_on_coordinates( p_pic, portal, i, y );
            }
        }

        if( p_bottomColor.is_non_empty( ) ) {
            auto portal_inner = vertex::unselected_vertex( COLOR_FILL_WHITE, inner, .75,
                                                           vertex::ST_TRIANGLE_DOWN );
            auto portal       = vertex::marked_vertex( p_bottomColor, vtxsz, .75, vertex::ST_CIRCLE,
                                                       portal_inner );
            auto [ x, y ]     = p_gs.m_bottomFrontier.front( );
            u64 xm            = x;
            for( const auto& pt : p_gs.m_bottomFrontier ) {
                if( pt.second == y ) {
                    xm = pt.first;
                } else {
                    break;
                }
            }
            for( auto i = xm; i <= x; ++i ) {
                p_vg.place_vertex_on_coordinates( p_pic, portal, i, y );
            }
        }
    }

    void place_graph_slices_stylized( picture& p_pic, const std::deque<graph_slice>& p_gs,
                                      breakpoint_repn p_alignment, color p_portalVertexColor,
                                      color p_innerPortalColor, color p_topVertexColor,
                                      color p_bottomVertexColor, color p_pureColor,
                                      color p_impureColor, color p_leftColor, color p_rightColor,
                                      double p_sliceSpacing ) {
        fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                         p_gs.back( ).fragment_p( ).open_end( ) };
        auto       align = breakpoint_slice( p_alignment, frag );

        for( u64 i = 0; i < p_gs.size( ); ++i ) {
            const auto& slice = p_gs[ i ];
            auto        fP    = slice.fragment_p( );
            auto        fPo   = slice.original_fragment_p( );
            vertex_grid vg{ tikz_point{ p_sliceSpacing * i, 0.0 },
                            tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT } };
            vg.place_coordinates( p_pic, slice.width( ) + 1, slice.height( ) + 1,
                                  slice.top_left( ).first, slice.top_left( ).second );

            auto bgcol = p_impureColor;
            if( !!( slice.m_flags & graph_slice::GSF_PURE ) ) { bgcol = p_pureColor; }
            if( !!( slice.m_flags & graph_slice::GSF_TOP ) ) { bgcol = p_leftColor; }
            if( !!( slice.m_flags & graph_slice::GSF_BOT ) ) { bgcol = p_rightColor; }

            place_graph_slice_on_coordinates(
                p_pic, vg, slice, bgcol, p_portalVertexColor, p_innerPortalColor,
                fP.closed_begin( ) == frag.closed_begin( ) ? p_topVertexColor : COLOR_NONE,
                fP.closed_end( ) == frag.closed_end( ) ? p_bottomVertexColor : COLOR_NONE );

            auto brpnt = breakpoint_slice( align, fPo );
            place_alignment_on_coordinates( p_pic, vg, brpnt );
        }
    }

    void place_graph_slices( picture& p_pic, const std::deque<graph_slice>& p_gs,
                             breakpoint_repn p_alignment, const std::string& p_P,
                             const std::string& p_T, color p_portalVertexColor,
                             color p_innerPortalColor, color p_topVertexColor,
                             color p_bottomVertexColor, double p_sliceSpacing ) {
        fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                         p_gs.back( ).fragment_p( ).open_end( ) };
        auto       align = breakpoint_slice( p_alignment, frag );

        for( u64 i = 0; i < p_gs.size( ); ++i ) {
            const auto& slice = p_gs[ i ];
            auto        fP    = slice.fragment_p( );
            auto        fPo   = slice.original_fragment_p( );

            vertex_grid vg;
            for( u64 j = 1; j < slice.m_topFrontier.size( ); ++j ) {

                auto fp2 = fragmentco{
                    slice.m_topFrontier[ j - 1 ].second,
                    slice.m_bottomFrontier[ slice.m_bottomFrontier.size( ) - j ].second };
                auto ft2 = fragmentco{ slice.m_topFrontier[ j - 1 ].first,
                                       slice.m_topFrontier[ j ].first };

                vg = place_alignment_graph(
                    p_pic, p_P, fp2, p_T, ft2, tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                    tikz_point{ p_sliceSpacing * i, 0.0 }, COLOR_BLACK.deemphasize( ) );
            }

            place_graph_slice_on_coordinates(
                p_pic, vg, slice, COLOR_NONE, p_portalVertexColor, p_innerPortalColor,
                fP.closed_begin( ) == frag.closed_begin( ) ? p_topVertexColor : COLOR_NONE,
                fP.closed_end( ) == frag.closed_end( ) ? p_bottomVertexColor : COLOR_NONE );

            auto brpnt = breakpoint_slice( align, fPo );
            place_alignment_on_coordinates( p_pic, vg, brpnt );
        }
    }

    void place_graph_slices_t_labels( picture& p_pic, const std::deque<graph_slice>& p_gs,
                                      const stylized_string& p_Tname,
                                      const stylized_string& p_Pname, bool p_labelEqualParts,
                                      double p_labelDis, double p_sliceSpacing ) {
        fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                         p_gs.back( ).fragment_p( ).open_end( ) };
        for( u64 i = 0; i < p_gs.size( ); ++i ) {
            const auto& slice = p_gs[ i ];
            auto        fT    = slice.fragment_t( );
            auto        fPo   = slice.original_fragment_p( );

            auto posT
                = tikz_point{ p_sliceSpacing * i + CHAR_WIDTH * fT.closed_begin( ),
                              frag.closed_begin( ) * -CHAR_HEIGHT + CHAR_HEIGHT + p_labelDis };
            auto posTB = tikz_point{ p_sliceSpacing * i + CHAR_WIDTH * fT.closed_begin( ),
                                     frag.open_end( ) * -CHAR_HEIGHT - p_labelDis };

            auto strT = p_Tname.slice( fT );
            place_string( p_pic, strT, posT );

            if( p_labelEqualParts && !!( slice.m_flags & graph_slice::GSF_PURE ) ) {
                strT = p_Pname.slice( fPo );
            } else {
                strT.m_color = strT.m_color.deemphasize( );
            }
            place_string( p_pic, strT, posTB );
        }
    }

    void place_graph_slices_p_labels( picture& p_pic, const std::deque<graph_slice>& p_gs, u64 p_d,
                                      bool p_guideLines, double p_labelDis,
                                      double p_sliceSpacing ) {

        fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                         p_gs.back( ).fragment_p( ).open_end( ) };
        fragmentco fragT{ p_gs.front( ).fragment_t( ).closed_begin( ),
                          p_gs.back( ).fragment_t( ).open_end( ) };
        for( s32 i = p_gs.size( ) - 1; i >= 0; --i ) {
            const auto& slice = p_gs[ i ];
            auto        fP    = slice.fragment_p( );
            auto        fPo   = slice.original_fragment_p( );
            auto        fT    = slice.fragment_t( );

            auto posP = tikz_point{ fragT.closed_begin( ) * CHAR_WIDTH - CHAR_WIDTH - p_labelDis
                                        - CHAR_WIDTH * i / 6.25,
                                    fP.closed_begin( ) * -CHAR_HEIGHT };
            std::string label;
            if( fPo.length( ) == 1 ) {
                label = std::string{ "P_{" } + std::to_string( p_d ) + ","
                        + std::to_string( fPo.closed_begin( ) ) + "}";
            } else {
                label = std::string{ "P\\fragmentco{p^{<" } + std::to_string( p_d ) + "}_"
                        + std::to_string( fPo.closed_begin( ) ) + "}{ p^{>" + std::to_string( p_d )
                        + "}_" + std::to_string( fPo.open_end( ) ) + "}";
            }
            stylized_string strP = stylized_string{ label, fP, str_displ_t::NAME };
            if( fPo.length( ) > 1 ) {
                strP.m_displayStyle = str_displ_t::NAME_ROTATE;
            } else {
                strP.m_labelAlign = AN_END;
                strP.highlight_position( i - fP.closed_begin( ), SEP_COL.deemphasize_weak( ) );
            }
            if( i ) { strP.m_color = strP.m_color.deemphasize( ); }
            place_string_vertical( p_pic, strP, posP );

            if( p_guideLines ) {
                // guide lines
                vertex_grid vg{ tikz_point{ p_sliceSpacing * i, 0.0 },
                                tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT } };
                if( fP.closed_begin( ) && i + 0LU != frag.closed_end( ) ) {
                    posP = tikz_point{ fragT.open_end( ) * CHAR_WIDTH
                                           + ( p_gs.size( ) - 1 ) * p_sliceSpacing + p_labelDis
                                           + CHAR_WIDTH * ( p_gs.size( ) - 1 - i ) / 6.25,
                                       fP.closed_begin( ) * -CHAR_HEIGHT };
                    p_pic.place_line( vg.point_for_pos( fT.open_end( ), fP.closed_begin( ) )
                                          + tikz_point{ .2, 0.0 },
                                      posP + tikz_point{ -.2, 0.0 },
                                      OPT::LW_SUPPORT_LINE
                                          | ( i % 5 ? OPT::DRAW( COLOR_TEXT.deemphasize_strong( ) )
                                                    : OPT::DRAW( COLOR_TEXT.deemphasize( ) ) ) );
                }
                if( i && fP.open_end( ) != frag.open_end( ) ) {
                    posP = tikz_point{ fragT.closed_begin( ) * CHAR_WIDTH - p_labelDis
                                           - CHAR_WIDTH * i / 6.25,
                                       fP.open_end( ) * -CHAR_HEIGHT };
                    p_pic.place_line( vg.point_for_pos( fT.closed_begin( ), fP.open_end( ) )
                                          + tikz_point{ -.2, 0.0 },
                                      posP + tikz_point{ .2, 0.0 },
                                      OPT::LW_SUPPORT_LINE
                                          | ( i % 5 ? OPT::DRAW( COLOR_TEXT.deemphasize_strong( ) )
                                                    : OPT::DRAW( COLOR_TEXT.deemphasize( ) ) ) );
                }
            }
        }

        for( u64 i = 0; i < p_gs.size( ); ++i ) {
            const auto& slice = p_gs[ i ];
            auto        fP    = slice.fragment_p( );
            auto        fPo   = slice.original_fragment_p( );
            auto        posP  = tikz_point{ fragT.open_end( ) * CHAR_WIDTH + p_labelDis
                                        + ( p_gs.size( ) - 1 ) * p_sliceSpacing
                                        + CHAR_WIDTH * ( p_gs.size( ) - 1 - i ) / 6.25,
                                    fP.closed_begin( ) * -CHAR_HEIGHT };
            std::string label;
            if( fPo.length( ) == 1 ) {
                label = std::string{ "P_{" } + std::to_string( p_d ) + ","
                        + std::to_string( fPo.closed_begin( ) ) + "}";
            } else {
                label = std::string{ "P" };
            }
            stylized_string strP = stylized_string{ label, fP, str_displ_t::NAME };
            if( fPo.length( ) > 1 ) {
                strP.m_displayStyle = str_displ_t::FRAGMENT;
            } else {
                strP.m_labelAlign = AN_BEGIN;
                strP.highlight_position( i - fP.closed_begin( ), SEP_COL.deemphasize( ) );
            }

            if( i != frag.closed_end( ) ) {
                strP.m_color = strP.m_color.deemphasize_strong( );
            } else {
                strP.m_color = strP.m_color.deemphasize( );
            }
            place_string_vertical( p_pic, strP, posP );
        }
    }
} // namespace TIKZ
