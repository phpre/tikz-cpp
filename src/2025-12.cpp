#include "tikz.h"
using namespace TIKZ;

std::string OUT_DIR      = "../figs/";
std::string TEX_DIR      = "../tex/";
std::string FONT_PATH    = TEX_DIR + "font_talk";
std::string COLOR_PATH   = TEX_DIR + "color_talk";
std::string MACRO_PATH   = TEX_DIR + "macros";
std::string PROGRAM_NAME = "";

constexpr std::string T = "AAAcdaAAAaacAAaddAAAAA";
constexpr std::string P = "AbAAbcdAAAdAAbddAAaAAA";

constexpr std::string T2 = "DEFGHaJKLMcOPQAB";
constexpr std::string P2 = "DEFGHIJKLMNOPQAB";

const stylized_string S_NAME{ std::string{ "S" }, fragmentco{ 0, 0 } };
const stylized_string T_NAME2{ std::string{ "T" }, fragmentco{ 0, T.size( ) } };
const stylized_string P_NAME2{ std::string{ "P" }, fragmentco{ 0, P.size( ) } };

const stylized_string T_NAME{ T, std::string{ "T" }, str_displ_t::FRAGMENT };
const stylized_string P_NAME{ P, std::string{ "P" }, str_displ_t::FRAGMENT };
const stylized_string T2_NAME{ T2, std::string{ "T" }, str_displ_t::FRAGMENT };
const stylized_string P2_NAME{ P2, std::string{ "P" }, str_displ_t::FRAGMENT };

const breakpoint_repn BP_P_T   = compute_breakpoints( P, T );
const breakpoint_repn BP_P2_T2 = compute_breakpoints( P2, T2 );

void alignment_picture( picture& p_pic, const std::string& p_P, const std::string& p_T ) {

    auto tn
        = stylized_string{ p_P, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn
        = stylized_string{ p_T, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    breakpoint_repn bp = compute_breakpoints( p_P, p_T );
    place_alignment( p_pic, pn, tikz_point{ 0.0, 0.0 }, tn, tikz_point{ 0.0, 1.25 }, bp,
                     AT_COMPRESS );
}

void real_alignments_picture( const std::string& p_name = "g20.tex" ) {
    document out{ };
    picture  p1{ };
    alignment_picture( p1, "saarbrücken", "saarbruecken" );
    out.add_picture( p1 );
    picture p2{ };
    alignment_picture( p2, "saarbrücken", "sarrebruck" );
    out.add_picture( p2 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void alignment_picture( const std::string& p_name   = "g21.tex",
                        breakpoint_repn p_alignment = BP_P2_T2, stylized_string p_Tname = T2_NAME,
                        stylized_string p_Pname = P2_NAME ) {
    document out{ };

    picture p1{ };
    place_alignment( p1, p_Pname, tikz_point{ 0.0, 0.0 }, p_Tname, tikz_point{ 0.0, 1.25 },
                     p_alignment, AT_COMPRESS );
    out.add_picture( p1 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void alignment_graph_picture(
    const std::string& p_name = "g22.tex",
    stylized_string    p_Tname
    = stylized_string{ T_NAME.m_name, T_NAME.m_fragment, str_displ_t ::POSITION },
    stylized_string p_Pname
    = stylized_string{ P_NAME.m_name, P_NAME.m_fragment, str_displ_t ::POSITION },
    fragmentco p_fragment = fragmentco{ 0, 11 }, breakpoint_repn p_alignment = BP_P_T ) {

    document out{ };
    picture  p1{ OPT::YSCALE( ".93" ) };
    {
        auto fragT = align_fragment( p_alignment, p_fragment );

        place_alignment_graph_label( p1, p_Pname.slice( p_fragment ), p_Tname.slice( fragT ) );
        auto vg = place_alignment_graph( p1, P, p_fragment, T, fragT );
        // highlight main diagonal
        vg.place_diagonal_on_coordinates( p1, vertex ::marked_vertex( SEP_COL.deemphasize_weak( ) ),
                                          1 + std::min( p_fragment.length( ), fragT.length( ) ) );
        // print alignment
        auto brpnt = breakpoint_slice( p_alignment, p_fragment );
        place_alignment_on_coordinates( p1, vg, brpnt );
    }
    out.add_picture( p1 );
    picture p2{ OPT::YSCALE( ".93" ) };
    {
        place_string_vertical(
            p2, stylized_string{ "P\\position{p}", fragmentco{ 0, 1 }, str_displ_t ::NAME },
            tikz_point{ -CHAR_WIDTH - .4, -.5 * CHAR_HEIGHT } );
        picture scope{ { }, {} };
        scope.place_rectangle( tikz_point{ -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT },
                               tikz_point{ .67 * CHAR_WIDTH, -2.67 * CHAR_HEIGHT }, OPT::CLIP );
        auto vg = place_alignment_graph(
            scope, "PPP", { 0, 3 }, "TTT", { 0, 2 }, tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
            tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH }, COLOR_TEXT );
        place_selected_arrow( scope, vg.label_for_pos( 1, 1 ), vg.label_for_pos( 1, 2 ), DEL_COL,
                              DEL_COL.to_bg( ), -90.0 );
        p2.add_scope( scope );
        p2.place_text( math_mode( "\\small w( P\\position{p} \\to \\varepsilon )" ),
                       tikz_point{ 2 * CHAR_WIDTH, -1 * CHAR_HEIGHT },
                       OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" ) | OPT::TEXT_COLOR( DEL_COL )
                           | OPT::INNER_SEP( "1pt" ) );
    }
    out.add_picture( p2 );
    picture p3{ OPT::YSCALE( ".93" ) };
    {
        place_string( p3,
                      stylized_string{ "T\\position{t}", fragmentco{ 0, 1 }, str_displ_t ::NAME },
                      tikz_point{ .5 * CHAR_WIDTH, CHAR_HEIGHT + .4 } );

        picture scope{ { }, {} };
        scope.place_rectangle( tikz_point{ -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT },
                               tikz_point{ 2.67 * CHAR_WIDTH, -.67 * CHAR_HEIGHT }, OPT::CLIP );
        auto vg = place_alignment_graph(
            scope, "PPP", { 0, 2 }, "TTT", { 0, 3 }, tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
            tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH }, COLOR_TEXT );
        place_selected_arrow( scope, vg.label_for_pos( 1, 1 ), vg.label_for_pos( 2, 1 ), INS_COL,
                              INS_COL.to_bg( ), -0.0 );
        p3.add_scope( scope );
        p3.place_text( math_mode( "\\small w( \\varepsilon \\to T\\position{t} )" ),
                       tikz_point{ 1 * CHAR_WIDTH, -1.25 * CHAR_HEIGHT },
                       OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" ) | OPT::TEXT_COLOR( INS_COL )
                           | OPT::INNER_SEP( "1pt" ) );
    }
    out.add_picture( p3 );
    picture p4{ OPT::YSCALE( ".93" ) };
    {
        place_string( p4,
                      stylized_string{ "T\\position{t}", fragmentco{ 0, 1 }, str_displ_t ::NAME },
                      tikz_point{ .5 * CHAR_WIDTH, CHAR_HEIGHT + .4 } );
        place_string_vertical(
            p4, stylized_string{ "P\\position{p}", fragmentco{ 0, 1 }, str_displ_t ::NAME },
            tikz_point{ -CHAR_WIDTH - .4, -.5 * CHAR_HEIGHT } );

        picture scope{ { }, {} };
        scope.place_rectangle( tikz_point{ -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT },
                               tikz_point{ 2.67 * CHAR_WIDTH, -2.67 * CHAR_HEIGHT }, OPT::CLIP );
        auto vg = place_alignment_graph(
            scope, "PPP", { 0, 3 }, "TTT", { 0, 3 }, tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
            tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH }, COLOR_TEXT );
        p4.add_scope( scope );
        p4.place_text( math_mode( "\\small w( P\\position{p} \\to T\\position{t})" ),
                       tikz_point{ 1.3 * CHAR_WIDTH, -.7 * CHAR_HEIGHT },
                       OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" ) | OPT::TEXT_COLOR( SUB_COL )
                           | OPT::INNER_SEP( "1pt" ) | OPT::ANCHOR_WEST );
        p4.place_text( math_mode( "\\small 0" ), tikz_point{ .7 * CHAR_WIDTH, -1.3 * CHAR_HEIGHT },
                       OPT::FILL( COLOR_WHITE ) | OPT::XSCALE( ".8" ) | OPT::TEXT_COLOR( MAT_COL )
                           | OPT::INNER_SEP( "1pt" ) | OPT::ANCHOR_EAST );

        place_selected_arrow( p4, vg.label_for_pos( 1, 1 ) + ") + (0.0, -.05",
                              vg.label_for_pos( 2, 2 ) + ") + ( 0.0, -.05", MAT_COL,
                              MAT_COL.to_bg( ), -45.0 );

        place_selected_arrow( p4, vg.label_for_pos( 1, 1 ) + ") + (0.0, +.05",
                              vg.label_for_pos( 2, 2 ) + ") + ( 0.0, +.05", SUB_COL,
                              SUB_COL.to_bg( ), -45.0 );
    }
    out.add_picture( p4 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void slices_picture( const std::string& p_name = "g23.tex", stylized_string p_Pname = P_NAME,
                     stylized_string p_Tname = S_NAME, fragmentco p_fragment = fragmentco{ 0, 11 },
                     u64 p_d = 1, breakpoint_repn p_alignment = BP_P_T, const std::string& p_P = P,
                     const std::string& p_T = T ) {
    auto pname_s = stylized_string{ p_Pname.m_name, p_Pname.m_fragment, str_displ_t ::NAME };
    auto tname_s = stylized_string{ p_Tname.m_name, p_Tname.m_fragment, str_displ_t ::NAME };

    auto align = breakpoint_slice( p_alignment, p_fragment );
    auto gs    = graph_slice::from_alignment( align, p_d );

    // full picture of every slice
    document out{ };
    picture  p1{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices( p1, gs, p_alignment, p_P, p_T, COLOR_BLACK.deemphasize( ), COLOR_NONE,
                            COLOR_NONE, COLOR_NONE );
        place_graph_slices_t_labels( p1, gs, p_Tname, p_Pname );
        // place_graph_slices_p_labels( p1, gs, p_d );
        place_string_vertical( p1, pname_s.slice( p_fragment ),
                               tikz_point{ -CHAR_WIDTH - .25, .0 } );
    }
    out.add_picture( p1 );
    picture p2{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices_stylized( p2, gs, p_alignment );
        place_graph_slices_t_labels( p2, gs, p_Tname, p_Pname, true );
        // place_graph_slices_p_labels( p2, gs, p_d );
        place_string_vertical( p2, pname_s.slice( p_fragment ),
                               tikz_point{ -CHAR_WIDTH - .25, .0 } );
    }
    out.add_picture( p2 );

    // left-pure-right decomposition
    auto    tpb_decomp = graph_slice::top_pure_bot_decomp( gs );
    picture p3{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices( p3, tpb_decomp, p_alignment, p_P, p_T, COLOR_BLACK.deemphasize( ),
                            COLOR_NONE, COLOR_NONE );
        place_graph_slices_t_labels( p3, tpb_decomp, p_Tname, p_Pname, true );
        place_string_vertical( p3, pname_s.slice( p_fragment ),
                               tikz_point{ -CHAR_WIDTH - .25, .0 } );
    }
    out.add_picture( p3 );
    picture p4{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices_stylized( p4, tpb_decomp, p_alignment );
        place_graph_slices_t_labels( p4, tpb_decomp, p_Tname, p_Pname, true );
        place_string_vertical( p4, pname_s.slice( p_fragment ),
                               tikz_point{ -CHAR_WIDTH - .25, .0 } );
    }
    out.add_picture( p4 );

    // fully merged
    auto    merged = graph_slice::merged_slice( gs, p_fragment );
    picture p5{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices( p5, { merged }, p_alignment, p_P, p_T, COLOR_NONE,
                            COLOR_BLACK.deemphasize( ), COLOR_NONE, COLOR_NONE );
        place_alignment_graph_label( p5, pname_s.slice( p_fragment ),
                                     tname_s.slice( merged.fragment_t( ) ) );
    }
    out.add_picture( p5 );
    picture p6{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices_stylized( p6, { merged }, p_alignment, COLOR_BLACK.deemphasize( ),
                                     COLOR_NONE, SEP_COL.deemphasize( ), SEP_COL.deemphasize( ),
                                     MAT_COL.deemphasize( ), COLOR_BLACK.deemphasize( ) );
        place_alignment_graph_label( p6, pname_s.slice( p_fragment ),
                                     tname_s.slice( merged.fragment_t( ) ) );
    }
    out.add_picture( p6 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_string( const std::string& p_name = "g03.tex", u64 p_n = 12, u64 p_m = 3 ) {
    auto tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };
    auto pn = stylized_string{ "P", fragmentco{ 0, p_m }, str_displ_t::NAME };

    document out{ };
    picture  p1{ };

    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p1, pn, tikz_point{ ( p_n - p_m ) / 2.0 * CHAR_WIDTH, -1.5 * CHAR_HEIGHT } );

    out.add_picture( p1 );
    picture p2{ };

    place_string( p2, tn.color_invert( ), tikz_point{ 0.0, 0.0 } );
    place_string( p2, pn.color_invert( ),
                  tikz_point{ ( p_n - p_m ) / 2.0 * CHAR_WIDTH, -1.5 * CHAR_HEIGHT } );

    out.add_picture( p2 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_filter_verify( const std::string& p_name = "g02.tex", u64 p_n = 12, u64 p_m = 3,
                            u64 p_k = 1, u64 p_srand = 0, u64 p_batch = 3 ) {
    srand( p_srand );

    auto     tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };
    document out{ };
    picture  p1{ };

    p1.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
    }

    out.add_picture( p1 );
    picture p2{ };
    p2.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
    place_string( p2, tn, tikz_point{ 0.0, 0.0 } );
    std::set<u64> left{ };
    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( rand( ) % 13 < 6 ) {
            p2.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            left.insert( i );
            p2.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        }
    }
    out.add_picture( p2 );

    picture p3{ };
    p3.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
    place_string( p3, tn, tikz_point{ 0.0, 0.0 } );
    std::set<u64> good{ };
    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( left.count( i ) ) {
            if( rand( ) % 7 < 3 ) {
                p3.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
            } else {
                good.insert( i );
                p3.place_maru( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
            }
        } else {
            p3.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                            COLOR_C3.deemphasize_weak( ).deemphasize( ),
                            COLOR_C3.deemphasize( ).deemphasize( ) );
        }
    }
    out.add_picture( p3 );

    for( auto n : left ) {
        picture p4{ };
        p4.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );

        for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
            if( left.count( i ) ) {
                if( i < n && good.count( i ) ) {
                    p4.place_maru( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                } else if( i < n ) {
                    p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                } else if( i == n ) {
                    p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                     COLOR_C2.deemphasize_weak( ), COLOR_C2.to_bg( ),
                                     OPT::FILL( COLOR_C5.to_bg( ) ) | OPT::RECTANGLE
                                         | OPT::ROUNDED_CORNERS );
                } else {
                    p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                     COLOR_C2.deemphasize_weak( ).deemphasize( ),
                                     COLOR_C2.to_bg( ).deemphasize( ) );
                }
            } else {
                p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                COLOR_C3.deemphasize_weak( ).deemphasize( ),
                                COLOR_C3.deemphasize( ).deemphasize( ) );
            }
        }
        place_string( p4, tn, tikz_point{ 0.0, 0.0 } );
        out.add_picture( p4 );
    }

    while( left.size( ) % p_batch ) { left.insert( p_n + left.size( ) ); }

    std::set<u64> batch{ };
    for( auto n : left ) {
        batch.insert( n );
        if( batch.size( ) < p_batch ) { continue; }

        picture p4{ };
        p4.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );

        for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
            if( left.count( i ) ) {
                if( i < *batch.begin( ) && good.count( i ) ) {
                    p4.place_maru( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                } else if( i < *batch.begin( ) ) {
                    p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                } else if( batch.count( i ) ) {
                    p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                     COLOR_C2.deemphasize_weak( ), COLOR_C2.to_bg( ),
                                     OPT::FILL( COLOR_C5.to_bg( ) ) | OPT::RECTANGLE
                                         | OPT::ROUNDED_CORNERS );
                } else {
                    p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                     COLOR_C2.deemphasize_weak( ).deemphasize( ),
                                     COLOR_C2.to_bg( ).deemphasize( ) );
                }
            } else {
                p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                COLOR_C3.deemphasize_weak( ).deemphasize( ),
                                COLOR_C3.deemphasize( ).deemphasize( ) );
            }
        }
        place_string( p4, tn, tikz_point{ 0.0, 0.0 } );
        out.add_picture( p4 );
        batch.clear( );
    }

    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_standard_trick( const std::string& p_name = "g01.tex", u64 p_n = 12, u64 p_m = 3,
                             u64 p_k = 1, u64 p_l = 4 ) {
    auto tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };
    auto pn = stylized_string{ "P", fragmentco{ 0, p_m }, str_displ_t::NAME };

    document        out{ };
    picture         p1{ };
    u64             fa1s = 3;
    breakpoint_repn fake_al1{
        breakpoint{ 0, fa1s },
        breakpoint{ p_m, fa1s + p_m, 0, 'a' },
        breakpoint{ p_m, fa1s + p_m + 1 },
    };
    p1.place_node( tikz_point{ ( 1 + p_n ) * CHAR_WIDTH, -3.5 * CHAR_HEIGHT } );
    p1.place_node( tikz_point{ ( -1 ) * CHAR_WIDTH, 0.0 } );
    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( i == fa1s ) {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                             COLOR_C2.deemphasize_weak( ).deemphasize( ),
                             COLOR_C2.to_bg( ).deemphasize( ) );
        }
    }
    place_string( p1, pn, tikz_point{ fa1s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    out.add_picture( p1 );

    place_highlighted_occurrence( p1, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                  tikz_point{ 0.0, 0.0 }, { fake_al1 }, 0 );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p1, pn, tikz_point{ fa1s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );

    out.add_picture( p1 );

    picture         p2{ };
    u64             fa2s = 3;
    breakpoint_repn fake_al2{
        breakpoint{ 0, fa2s },
        breakpoint{ p_m - 2, fa2s + p_m - 2, 'a', 0 },
        breakpoint{ p_m, fa2s + p_m - 1 },
    };
    p2.place_node( tikz_point{ ( 1 + p_n ) * CHAR_WIDTH, -3.5 * CHAR_HEIGHT } );
    p2.place_node( tikz_point{ ( -1 ) * CHAR_WIDTH, 0.0 } );

    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( i == fa2s ) {
            p2.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            p2.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                             COLOR_C2.deemphasize_weak( ).deemphasize( ),
                             COLOR_C2.to_bg( ).deemphasize( ) );
        }
    }

    place_highlighted_occurrence( p2, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                  tikz_point{ 0.0, 0.0 }, { fake_al2 }, 0 );
    place_string( p2, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p2, pn, tikz_point{ fa2s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
    out.add_picture( p2 );

    u64             fa3s = p_n - p_m + 1;
    breakpoint_repn fake_al3{
        breakpoint{ 0, fa3s },
        breakpoint{ p_m - 1, fa3s + p_m - 1, 'a', 0 },
        breakpoint{ p_m, fa3s + p_m - 1 },
    };

    p1.place_node( tikz_point{ ( 1 + p_n ) * CHAR_WIDTH, -3.5 * CHAR_HEIGHT } );
    p1.place_node( tikz_point{ ( -1 ) * CHAR_WIDTH, 0.0 } );

    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( i == fa3s ) {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                             COLOR_C2.deemphasize_weak( ).deemphasize( ),
                             COLOR_C2.to_bg( ).deemphasize( ) );
        }
    }

    place_highlighted_occurrence( p1, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                  tikz_point{ 0.0, 0.0 }, { fake_al3 }, 0 );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p1, pn, tikz_point{ fa3s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
    out.add_picture( p1 );

    // split into substrings
    for( u64 j = 0; j < p_n - p_m + p_k + 1; j += p_l ) {
        picture     p4{ };
        std::string name = "T\\fragmentco{";
        if( !j ) {
            name += "0";
        } else if( j / p_l == 1 ) {
            name += "\\ell";
        } else {
            name += std::format( "{}\\ell", j / p_l );
        }
        name += "}{";
        if( !j ) {
            name += " k + p + \\ell";
        } else if( j + p_l >= p_n - p_m + p_k + 1 ) {
            name += "t";
        } else {
            name += std::format( " k + p + {}\\ell", 1 + ( j / p_l ) );
        }
        name += "}";

        auto tn = stylized_string{ name, fragmentco{ j, std::min( p_n, j + p_m + p_l + p_k - 1 ) },
                                   str_displ_t::NAME };

        for( u64 i = j; i < std::min( p_n - p_m + p_k + 1, j + p_l ); ++i ) {
            if( i == fa3s ) {
                p4.place_hatena( tikz_point{ ( i - j + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
            } else {
                p4.place_hatena( tikz_point{ ( i - j + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                 COLOR_C2.deemphasize_weak( ).deemphasize( ),
                                 COLOR_C2.to_bg( ).deemphasize( ) );
            }
        }

        if( j <= fa3s and fa3s < j + p_l ) {
            place_highlighted_occurrence( p4, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                          tikz_point{ 0.0, 0.0 }, { fake_al3 }, 0 );
            place_string( p4, pn, tikz_point{ ( fa3s - j ) * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
        }
        if( j <= fa1s and fa1s < j + p_l ) {
            place_highlighted_occurrence( p4, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                          tikz_point{ 0.0, 0.0 }, { fake_al1 }, 0 );
            place_string( p4, pn, tikz_point{ ( fa1s - j ) * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
        }

        place_string( p4, tn, tikz_point{ 0.0, 0.0 } );

        out.add_picture( p4 );
    }

    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_structural_insight1( const std::string& p_name = "g11.tex",
                                  occ_style_t p_style = occ_style_t::OCC_FULL, u64 p_n = 16,
                                  u64 p_m = 8, u64 p_k = 3 ) {
    // T aaaabbbb
    // P   aabb

    std::string T{ }, P{ };
    for( u64 i = 0; i < p_n / 2; ++i ) { T += "a"; }
    for( u64 i = 0; i < p_m / 2; ++i ) { P += "a"; }

    for( u64 i = 0; i < p_n / 2; ++i ) { T += "b"; }
    for( u64 i = 0; i < p_m / 2; ++i ) { P += "b"; }

    auto occs = compute_occs_with_mism( P, T, p_k );

    auto tn = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    for( u64 i = 0; i < p_n / 2; ++i ) { tn.annotation_at_pos( p_n / 2 + i ).m_wasWildcard = true; }
    for( u64 i = 0; i < p_m / 2; ++i ) { pn.annotation_at_pos( p_m / 2 + i ).m_wasWildcard = true; }

    // reorder occs
    auto occsre = std::deque<breakpoint_repn>{ };
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT >= p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }
    std::reverse( occs.begin( ), occs.end( ) );
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT < p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }

    document out{ };
    add_occurrences_pictures( out, pn, tn, occsre, p_style );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_structural_insight2( const std::string& p_name = "g12.tex",
                                  occ_style_t p_style = occ_style_t::OCC_FULL, u64 p_n = 16,
                                  u64 p_m = 8, u64 p_k = 4, u64 p_srand = 0 ) {
    // T aabaababa
    // P   abab
    srand( p_srand );

    std::string T{ }, P{ };
    for( u64 i = 0; i < p_n; ++i ) { T += "a"; }
    for( u64 i = 0; i < p_m; ++i ) { P += "a"; }

    for( u64 i = 0; i < p_k / 2; ++i ) {
        T[ rand( ) % T.size( ) ] = 'b';
        P[ rand( ) % P.size( ) ] = 'b';
    }

    auto occs = compute_occs_with_mism( P, T, p_k );

    auto tn = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    for( u64 i = 0; i < p_n; ++i ) {
        if( T[ i ] == 'b' ) { tn.annotation_at_pos( i ).m_wasWildcard = true; }
    }
    for( u64 i = 0; i < p_m; ++i ) {
        if( P[ i ] == 'b' ) { pn.annotation_at_pos( i ).m_wasWildcard = true; }
    }

    document out{ };
    add_occurrences_pictures( out, pn, tn, occs, p_style );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

int main( int p_argc, char* p_argv[] ) {
    PROGRAM_NAME = p_argv[ 0 ];
    if( p_argc > 1 ) { OUT_DIR = std::string{ p_argv[ 1 ] }; }
    if( p_argc > 2 ) {
        TEX_DIR    = std::string{ p_argv[ 2 ] };
        FONT_PATH  = TEX_DIR + "font_talk";
        COLOR_PATH = TEX_DIR + "color_talk";
        MACRO_PATH = TEX_DIR + "macros";
    }

    CROSS_FILL = true;
    // real_alignments_picture( );
    picture_structural_insight1( );
    picture_structural_insight1( "g11b.tex", occ_style_t::NO_ANNOTATION );
    picture_structural_insight2( );
    picture_structural_insight2( "g12b.tex", occ_style_t::NO_ANNOTATION );

    CROSS_FILL = false;
    picture_standard_trick( );
    picture_filter_verify( );
    picture_string( );

    alignment_picture( );
    alignment_picture( "g21b.tex", breakpoint_slice( BP_P_T, { 0, BP_P2_T2.back( ).m_posP } ),
                       T_NAME2.slice( align_fragment( BP_P_T, { 0, BP_P2_T2.back( ).m_posP } ) ),
                       P_NAME2.slice( { 0, BP_P2_T2.back( ).m_posP } ) );
    alignment_graph_picture( );
    slices_picture( );
    slices_picture( "g23b.tex", P_NAME, P_NAME, fragmentco{ 0, 11 }, 1,
                    trivial_alignment( fragmentco{ 0, 11 } ), P, P );
    slices_picture( "g23c.tex", P_NAME, S_NAME, fragmentco{ 0, 11 }, 1, BP_P2_T2, P2, T2 );
}
