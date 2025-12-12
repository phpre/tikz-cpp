#include "tikz_default_paths.h"
#include "tikz_default_pictures.h"
#include "tikz_string.h"
#include "tikz_string_trie.h"

namespace TIKZ {
    void add_breakpoint_computation_pictures( document& p_doc, const std::string& p_T,
                                              stylized_string p_Tname, const std::string& p_P,
                                              stylized_string        p_Pname,
                                              const std::set<point>& p_filter,
                                              const std::string& p_wildcard, bool p_wcInOutput,
                                              bool p_skipInit, bool p_highlightInit,
                                              bool p_showSelection, bool p_highlightNextStep ) {
        auto bp = compute_breakpoints(
            p_P, p_T, p_wildcard, p_wcInOutput, [ & ]( u64 p_l_y, u64 p_l_x, dp_table p_l_dp ) {
                if( !p_filter.empty( ) && !p_filter.count( { p_l_y, p_l_x } ) ) { return; }

                bool lst = false;
                if( p_highlightNextStep && !p_l_x && p_l_y == p_P.size( ) ) {
                    lst = true;
                } else if( p_skipInit && ( !p_l_y || !p_l_x ) ) {
                    return;
                }
                WITH_PICTURE( pic, { }, p_doc ) {
                    place_alignment_graph_label( pic, p_Pname, p_Tname );
                    auto vg = vertex_grid{ tikz_point{ 0.0, 0.0 },
                                           tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT } };
                    vg.place_vertices( pic,
                                       vertex::unselected_vertex( COLOR_TEXT.to_bg( ), 1.0, 3.0 ),
                                       p_T.size( ) + 1, p_P.size( ) + 1, 0, 0 );

                    for( u64 y = 0; y <= p_P.size( ); ++y ) {
                        for( u64 x = 0; x <= p_T.size( ); ++x ) {
                            if( y > p_l_y && ( x || !p_l_y || !p_l_x ) ) { break; }
                            if( !p_l_x && y && x ) { break; }

                            if( p_showSelection ) {
                                if( y && x && p_l_dp[ y ][ x ] == p_l_dp[ y - 1 ][ x - 1 ]
                                    && p_P[ y - 1 ] == p_T[ x - 1 ] ) {
                                    // match
                                    place_arrow( pic, vg.label_for_pos( x - 1, y - 1 ),
                                                 vg.label_for_pos( x, y ),
                                                 OPT::LW_THICK_SUPPORT_LINE
                                                     | OPT::DRAW( MAT_COL.deemphasize_strong( ) ) );
                                }
                                if( y && x && p_l_dp[ y ][ x ] == 1 + p_l_dp[ y - 1 ][ x - 1 ]
                                    && p_P[ y - 1 ] != p_T[ x - 1 ] ) {
                                    // TODO: use cost( .. )
                                    // subst
                                    place_arrow( pic, vg.label_for_pos( x - 1, y - 1 ),
                                                 vg.label_for_pos( x, y ),
                                                 OPT::DRAW( SUB_COL.deemphasize_strong( ) ) );
                                }
                                if( y && p_l_dp[ y ][ x ] == 1 + p_l_dp[ y - 1 ][ x ] ) {
                                    // TODO: use cost( .. )
                                    // del
                                    place_arrow( pic, vg.label_for_pos( x, y - 1 ),
                                                 vg.label_for_pos( x, y ),
                                                 OPT::DRAW( DEL_COL.deemphasize_strong( ) ) );
                                }
                                if( x && p_l_dp[ y ][ x ] == 1 + p_l_dp[ y ][ x - 1 ] ) {
                                    // TODO: use cost( .. )
                                    // ins
                                    place_arrow( pic, vg.label_for_pos( x - 1, y ),
                                                 vg.label_for_pos( x, y ),
                                                 OPT::DRAW( INS_COL.deemphasize_strong( ) ) );
                                }
                            }

                            if( p_highlightInit && ( !y || !x ) ) {
                                place_uncircled_number( pic, vg.point_for_pos( x, y ),
                                                        p_l_dp[ y ][ x ], 4 * CHAR_WIDTH, SEP_COL,
                                                        SEP_COL.to_flavor_bg( ) );
                            } else {
                                place_uncircled_number( pic, vg.point_for_pos( x, y ),
                                                        p_l_dp[ y ][ x ], 4 * CHAR_WIDTH,
                                                        COLOR_TEXT, COLOR_TEXT.to_flavor_bg( ) );
                            }
                            if( y == p_l_y && x == p_l_x ) { break; }
                        }
                    }
                    if( p_highlightNextStep && ( lst || ( p_l_x && p_l_y ) ) ) {
                        u64 ny = p_l_y;
                        u64 nx = p_l_x + 1;
                        if( nx > p_T.size( ) ) {
                            ny++;
                            nx = 1;
                        }
                        if( lst ) { nx = ny = 1; }
                        if( ny > p_P.size( ) ) { continue; }
                        p_doc.add_picture( pic );
                        pic.place_node(
                            vg.point_for_pos( nx, ny ), EMPTY_STR,
                            OPT::DRAW( COLOR_TEXT.to_bg( ) ) | OPT::FILL( COLOR_TEXT.to_bg( ) )
                                | OPT::LW_SUPPORT_LINE | OPT::CIRCLE
                                | OPT::DOUBLE( COLOR_TEXT.to_bg( ) )
                                | OPT::INNER_SEP( std::format( "{:5.3f}pt", 4 * CHAR_WIDTH ) ) );

                        pic.place_double_text( textsize_footnotesize( "\\textbf{?}" ),
                                               vg.point_for_pos( nx, ny ), COLOR_C2.to_bg( ),
                                               COLOR_C2.deemphasize_weak( ), .25 );

                        if( p_P[ ny - 1 ] != p_T[ nx - 1 ] ) {
                            place_arrow( pic, vg.label_for_pos( nx - 1, ny - 1 ),
                                         vg.label_for_pos( nx, ny ),
                                         OPT::DRAW( SUB_COL.deemphasize( ) ) );
                        } else {
                            place_arrow( pic, vg.label_for_pos( nx - 1, ny - 1 ),
                                         vg.label_for_pos( nx, ny ),
                                         OPT::DRAW( MAT_COL.deemphasize( ) ) );
                        }

                        place_arrow( pic, vg.label_for_pos( nx, ny - 1 ),
                                     vg.label_for_pos( nx, ny ),
                                     OPT::DRAW( DEL_COL.deemphasize( ) ) );
                        place_arrow( pic, vg.label_for_pos( nx - 1, ny ),
                                     vg.label_for_pos( nx, ny ),
                                     OPT::DRAW( INS_COL.deemphasize( ) ) );
                    }
                }
            } );
    }

    void add_string_picture( document& p_out, const std::string& p_string ) {
        WITH_PICTURE( pic, { }, p_out ) {
            place_string( pic, p_string, tikz_point{ 0.0, 0.0 } );
        }
    }

    void add_occurrences_pictures( document& p_out, const stylized_string& p_pname,
                                   const stylized_string&             p_tname,
                                   const std::deque<breakpoint_repn>& p_occs,
                                   occ_style_t                        p_style ) {
        for( u64 y = 0; y < p_occs.size( ); ++y ) {
            WITH_PICTURE( pic, { }, p_out ) {
                place_highlighted_occurrence( pic, p_pname, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT },
                                              p_tname, tikz_point{ 0.0, 0.0 }, p_occs, y, p_style );
                pic.place_text( math_mode( VSIZE_CORRECTION + p_pname.m_name ),
                                tikz_point{ -1 * CHAR_WIDTH, -3 * CHAR_HEIGHT } );
                pic.place_text( math_mode( VSIZE_CORRECTION + p_tname.m_name ),
                                tikz_point{ -1 * CHAR_WIDTH, -CHAR_HEIGHT / 2 } );
            }
        }
    }

    void add_alignment_picture( document& p_out, const std::string& p_P, const std::string& p_T,
                                const cost_table& p_cost, alignment_style_t p_style ) {
        breakpoint_repn bp = compute_breakpoints( p_P, p_T, p_cost );
        add_alignment_picture( p_out, p_P, p_T, bp, p_style );
    }

    void add_alignment_picture( document& p_out, const std::string& p_P, const std::string& p_T,
                                breakpoint_repn p_alignment, alignment_style_t p_style ) {
        auto tn = stylized_string{ p_T, "T",
                                   str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
        auto pn = stylized_string{ p_P, "P",
                                   str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
        add_alignment_picture( p_out, p_alignment, tn, pn, p_style );
    }

    void add_alignment_picture( document& p_out, breakpoint_repn p_alignment,
                                stylized_string p_Tname, stylized_string p_Pname,
                                alignment_style_t p_style ) {
        WITH_PICTURE( pic, { }, p_out ) {
            place_alignment( pic, p_Pname, tikz_point{ 0.0, 0.0 }, p_Tname, tikz_point{ 0.0, 1.25 },
                             p_alignment, p_style );
        }
    }

    void add_alignment_graph_picture( document& p_doc, const std::string& p_T,
                                      stylized_string p_Tname, const std::string& p_P,
                                      stylized_string p_Pname, fragmentco p_fragment,
                                      breakpoint_repn p_alignment, bool p_highlightMD ) {
        WITH_PICTURE( p1, { OPT::YSCALE( ".93" ) }, p_doc ) {
            auto fragT = align_fragment( p_alignment, p_fragment );

            place_alignment_graph_label( p1, p_Pname.slice( p_fragment ), p_Tname.slice( fragT ) );
            auto vg = place_alignment_graph( p1, p_P, p_fragment, p_T, fragT );
            if( p_highlightMD ) {
                // highlight main diagonal
                vg.place_diagonal_on_coordinates(
                    p1, vertex ::marked_vertex( SEP_COL.deemphasize_weak( ) ),
                    1 + std::min( p_fragment.length( ), fragT.length( ) ) );
            }
            // print alignment
            auto brpnt = breakpoint_slice( p_alignment, p_fragment );
            place_alignment_on_coordinates( p1, vg, brpnt );
        }
    }

    void add_alignment_graph_explanation_pictures( document& p_doc, const std::string& p_Tname,
                                                   const std::string& p_Pname,
                                                   const std::string& p_Tposname,
                                                   const std::string& p_Pposname ) {
        std::string Pp = std::format( "{}\\position{{{}}}", p_Pname, p_Pposname );
        std::string Tp = std::format( "{}\\position{{{}}}", p_Tname, p_Tposname );

        WITH_PICTURE( p2, { OPT::YSCALE( ".93" ) }, p_doc ) {
            place_string_vertical( p2,
                                   stylized_string{ Pp, fragmentco{ 0, 1 }, str_displ_t ::NAME },
                                   tikz_point{ -CHAR_WIDTH - .4, -.5 * CHAR_HEIGHT } );
            picture scope{ { }, {} };
            scope.place_rectangle( tikz_point{ -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT },
                                   tikz_point{ .67 * CHAR_WIDTH, -2.67 * CHAR_HEIGHT }, OPT::CLIP );
            auto vg = place_alignment_graph( scope, "PPP", { 0, 3 }, "TTT", { 0, 2 },
                                             tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
                                             tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH },
                                             COLOR_TEXT );
            place_selected_arrow( scope, vg.label_for_pos( 1, 1 ), vg.label_for_pos( 1, 2 ),
                                  DEL_COL, DEL_COL.to_bg( ), -90.0 );
            p2.add_scope( scope );
            p2.place_text( math_mode( "\\small w( " + Pp + " \\to \\varepsilon )" ),
                           tikz_point{ 2 * CHAR_WIDTH, -1 * CHAR_HEIGHT },
                           OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" )
                               | OPT::TEXT_COLOR( DEL_COL ) | OPT::INNER_SEP( "1pt" ) );
        }
        WITH_PICTURE( p3, { OPT::YSCALE( ".93" ) }, p_doc ) {
            place_string( p3, stylized_string{ Tp, fragmentco{ 0, 1 }, str_displ_t ::NAME },
                          tikz_point{ .5 * CHAR_WIDTH, CHAR_HEIGHT + .4 } );

            picture scope{ { }, {} };
            scope.place_rectangle( tikz_point{ -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT },
                                   tikz_point{ 2.67 * CHAR_WIDTH, -.67 * CHAR_HEIGHT }, OPT::CLIP );
            auto vg = place_alignment_graph( scope, "PPP", { 0, 2 }, "TTT", { 0, 3 },
                                             tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
                                             tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH },
                                             COLOR_TEXT );
            place_selected_arrow( scope, vg.label_for_pos( 1, 1 ), vg.label_for_pos( 2, 1 ),
                                  INS_COL, INS_COL.to_bg( ), -0.0 );
            p3.add_scope( scope );
            p3.place_text( math_mode( "\\small w( \\varepsilon \\to " + Tp + " )" ),
                           tikz_point{ 1 * CHAR_WIDTH, -1.25 * CHAR_HEIGHT },
                           OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" )
                               | OPT::TEXT_COLOR( INS_COL ) | OPT::INNER_SEP( "1pt" ) );
        }
        WITH_PICTURE( p4, { OPT::YSCALE( ".93" ) }, p_doc ) {
            place_string( p4, stylized_string{ Tp, fragmentco{ 0, 1 }, str_displ_t ::NAME },
                          tikz_point{ .5 * CHAR_WIDTH, CHAR_HEIGHT + .4 } );
            place_string_vertical( p4,
                                   stylized_string{ Pp, fragmentco{ 0, 1 }, str_displ_t ::NAME },
                                   tikz_point{ -CHAR_WIDTH - .4, -.5 * CHAR_HEIGHT } );

            picture scope{ { }, {} };
            scope.place_rectangle( tikz_point{ -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT },
                                   tikz_point{ 2.67 * CHAR_WIDTH, -2.67 * CHAR_HEIGHT },
                                   OPT::CLIP );
            auto vg = place_alignment_graph( scope, "PPP", { 0, 3 }, "TTT", { 0, 3 },
                                             tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
                                             tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH },
                                             COLOR_TEXT );
            p4.add_scope( scope );
            p4.place_text( math_mode( "\\small w( " + Pp + " \\to " + Tp + " )" ),
                           tikz_point{ 1.3 * CHAR_WIDTH, -.7 * CHAR_HEIGHT },
                           OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" )
                               | OPT::TEXT_COLOR( SUB_COL ) | OPT::INNER_SEP( "1pt" )
                               | OPT::ANCHOR_WEST );
            p4.place_text(
                math_mode( "\\small 0" ), tikz_point{ .7 * CHAR_WIDTH, -1.3 * CHAR_HEIGHT },
                OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" ) | OPT::TEXT_COLOR( MAT_COL )
                    | OPT::INNER_SEP( "1pt" ) | OPT::ANCHOR_EAST );

            place_selected_arrow( p4, vg.label_for_pos( 1, 1 ) + ") + (0.0, -.05",
                                  vg.label_for_pos( 2, 2 ) + ") + ( 0.0, -.05", MAT_COL,
                                  MAT_COL.to_bg( ), -45.0 );

            place_selected_arrow( p4, vg.label_for_pos( 1, 1 ) + ") + (0.0, +.05",
                                  vg.label_for_pos( 2, 2 ) + ") + ( 0.0, +.05", SUB_COL,
                                  SUB_COL.to_bg( ), -45.0 );
        }
    }

    void add_slices_pictures( document& p_doc, stylized_string p_Pname, stylized_string p_Tname,
                              fragmentco p_fragment, u64 p_d, breakpoint_repn p_alignment,
                              const std::string& p_P, const std::string& p_T,
                              bool p_simplePlabel ) {
        auto pname_s = stylized_string{ p_Pname.m_name, p_Pname.m_fragment, str_displ_t ::NAME };
        auto tname_s = stylized_string{ p_Tname.m_name, p_Tname.m_fragment, str_displ_t ::NAME };

        auto align = breakpoint_slice( p_alignment, p_fragment );
        auto gs    = graph_slice::from_alignment( align, p_d );

        // full picture of every slice
        WITH_PICTURE( p1, { OPT::YSCALE( ".9" ) }, p_doc ) {
            place_graph_slices( p1, gs, p_alignment, p_P, p_T, COLOR_BLACK.deemphasize( ),
                                COLOR_NONE, COLOR_NONE, COLOR_NONE );
            place_graph_slices_t_labels( p1, gs, p_Tname, p_Pname );
            if( p_simplePlabel ) {
                place_string_vertical( p1, pname_s.slice( p_fragment ),
                                       tikz_point{ -CHAR_WIDTH - .25, .0 } );
            } else {
                place_graph_slices_p_labels( p1, gs, p_d );
            }
        }
        WITH_PICTURE( p2, { OPT::YSCALE( ".9" ) }, p_doc ) {
            place_graph_slices_stylized( p2, gs, p_alignment );
            place_graph_slices_t_labels( p2, gs, p_Tname, p_Pname, true );
            if( p_simplePlabel ) {
                place_string_vertical( p2, pname_s.slice( p_fragment ),
                                       tikz_point{ -CHAR_WIDTH - .25, .0 } );
            } else {
                place_graph_slices_p_labels( p2, gs, p_d );
            }
        }

        // left-pure-right decomposition
        auto tpb_decomp = graph_slice::top_pure_bot_decomp( gs );
        WITH_PICTURE( p3, { OPT::YSCALE( ".9" ) }, p_doc ) {
            place_graph_slices( p3, tpb_decomp, p_alignment, p_P, p_T, COLOR_BLACK.deemphasize( ),
                                COLOR_NONE, COLOR_NONE );
            place_graph_slices_t_labels( p3, tpb_decomp, p_Tname, p_Pname, true );
            place_string_vertical( p3, pname_s.slice( p_fragment ),
                                   tikz_point{ -CHAR_WIDTH - .25, .0 } );
        }
        WITH_PICTURE( p4, { OPT::YSCALE( ".9" ) }, p_doc ) {
            place_graph_slices_stylized( p4, tpb_decomp, p_alignment );
            place_graph_slices_t_labels( p4, tpb_decomp, p_Tname, p_Pname, true );
            place_string_vertical( p4, pname_s.slice( p_fragment ),
                                   tikz_point{ -CHAR_WIDTH - .25, .0 } );
        }

        // fully merged
        auto merged = graph_slice::merged_slice( gs, p_fragment );
        WITH_PICTURE( p5, { OPT::YSCALE( ".9" ) }, p_doc ) {
            place_graph_slices( p5, { merged }, p_alignment, p_P, p_T, COLOR_NONE,
                                COLOR_BLACK.deemphasize( ), COLOR_NONE, COLOR_NONE );
            place_alignment_graph_label( p5, pname_s.slice( p_fragment ),
                                         tname_s.slice( merged.fragment_t( ) ) );
        }
        WITH_PICTURE( p6, { OPT::YSCALE( ".9" ) }, p_doc ) {
            place_graph_slices_stylized( p6, { merged }, p_alignment, COLOR_BLACK.deemphasize( ),
                                         COLOR_NONE, SEP_COL.deemphasize( ), SEP_COL.deemphasize( ),
                                         MAT_COL.deemphasize( ), COLOR_BLACK.deemphasize( ) );
            place_alignment_graph_label( p6, pname_s.slice( p_fragment ),
                                         tname_s.slice( merged.fragment_t( ) ) );
        }
    }

    void add_trie_construction_pictures( document& p_doc, const std::string& p_alphabet,
                                         const std::deque<std::string>& p_str ) {
        trie T;
        for( const auto& s : p_str ) {
            picture p1{ };
            WITH_PICTURE( p1, { }, p_doc ) {
                T.insert( s );
                if( p_alphabet == EMPTY_STR ) {
                    auto tr = place_trie( p1, T );
                    place_trie_string_on_coordinates( p1, tr, s, OPT::COLOR( COLOR_C3 ) );
                    place_trie_depth_labels( p1, tr, tikz_point{ 0, CHAR_HEIGHT * 3 / 4 } );
                } else {
                    double charDiv = 1.25 * DEFAULT_TRIE_CHAR_DIVERTION;
                    double distX   = DEFAULT_TRIE_VERTEX_DIST_X;
                    double distY   = 1.15 * DEFAULT_TRIE_VERTEX_DIST_Y;
                    auto tr = place_trie_wide( p1, T, p_alphabet, tikz_point{ 0.0, 0.0 }, EMPTY_STR,
                                               distX, distY, charDiv );
                    place_diverted_trie_string_on_coordinates( p1, tr, s, p_alphabet, charDiv,
                                                               OPT::COLOR( COLOR_C3 ) );
                    place_trie_depth_labels( p1, tr, tikz_point{ 0, CHAR_HEIGHT * 3 / 4 } );
                }
            }
        }
    }

} // namespace TIKZ
