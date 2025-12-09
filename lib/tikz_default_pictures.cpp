#include "tikz_default_pictures.h"
#include "tikz_string.h"

namespace TIKZ {
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

    void add_alignment_graph_explanation_picture( document& p_doc, const std::string& p_Tname,
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

} // namespace TIKZ
