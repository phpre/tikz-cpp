#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

constexpr std::string T = "AAAcdaAAAaacAAaddAAAAA";
constexpr std::string P = "AbAAbcdAAAdAAbddAAaAAA";

constexpr std::string T2 = "ABCDEFGHaJKLMcOPQRSc";
constexpr std::string P2 = "ABCDEFGHIJKLMNOPQRST";

const stylized_string S_NAME{ std::string{ "S" }, fragmentco{ 0, 0 } };
const stylized_string T_NAME2{ std::string{ "T" }, fragmentco{ 0, T.size( ) } };
const stylized_string P_NAME2{ std::string{ "P" }, fragmentco{ 0, P.size( ) } };

const stylized_string T_NAME{ T, std::string{ "T" }, str_displ_t::FRAGMENT };
const stylized_string P_NAME{ P, std::string{ "P" }, str_displ_t::FRAGMENT };
const stylized_string T2_NAME{ T2, std::string{ "T" }, str_displ_t::FRAGMENT };
const stylized_string P2_NAME{ P2, std::string{ "P" }, str_displ_t::FRAGMENT };

const breakpoint_repn BP_P_T   = compute_breakpoints( P, T );
const breakpoint_repn BP_P2_T2 = compute_breakpoints( P2, T2 );

void alignment_picture( const std::string& p_name      = "g01.tex",
                        breakpoint_repn    p_alignment = BP_P_T ) {
    document out{ };

    picture p1{ };
    place_alignment( p1, P_NAME2, tikz_point{ 0.0, 0.0 }, T_NAME2, tikz_point{ 0.0, 1.25 },
                     p_alignment, true );
    out.add_picture( p1 );

    picture p2{ };
    auto    next
        = place_alignment( p2, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 },
                           breakpoint_slice( p_alignment, { 0, 6 } ), true );

    next = translate_right( next, 2.5 );
    next = place_alignment( p2, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 6, 10 } ), true );
    out.add_picture( p2 );

    picture p3{ };
    next = translate_right( next, -next.first.m_x );
    next = place_alignment( p3, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 13 } ), true );
    next = translate_right( next, 2.5 );
    next = place_alignment( p3, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 13, 22 } ), true );
    out.add_picture( p3 );

    picture p4{ };
    next = place_alignment( p4, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 },
                            breakpoint_slice( p_alignment, { 0, 1 } ), true );

    next = translate_right( next, 1 );
    next = place_alignment( p4, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 10 } ), true );

    next = translate_right( next, 1 );
    next = place_alignment( p4, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 11 } ), true );

    next = translate_right( next, 1 );
    next = place_alignment( p4, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 11, 11 } ), true );
    out.add_picture( p4 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void alignment_graph_picture(
    const std::string& p_name = "g02.tex",
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

void slices_picture( const std::string& p_name = "g03.tex", stylized_string p_Pname = P_NAME,
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
        place_graph_slices_p_labels( p1, gs, p_d );
    }
    out.add_picture( p1 );
    picture p2{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices_stylized( p2, gs, p_alignment );
        place_graph_slices_t_labels( p2, gs, p_Tname, p_Pname, true );
        place_graph_slices_p_labels( p2, gs, p_d );
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

void slices_detail_picture( const std::string& p_name = "g04.tex", stylized_string p_Pname = P_NAME,
                            stylized_string p_Tname = S_NAME,
                            fragmentco p_fragment = fragmentco{ 0, 11 }, u64 p_d = 1,
                            breakpoint_repn p_alignment = BP_P_T, const std::string& p_P = P,
                            const std::string& p_T = T ) {
    auto pname_s = stylized_string{ p_Pname.m_name, p_Pname.m_fragment, str_displ_t ::NAME };
    auto tname_s = stylized_string{ p_Tname.m_name, p_Tname.m_fragment, str_displ_t ::NAME };

    auto align = breakpoint_slice( p_alignment, p_fragment );
    auto gs    = graph_slice::from_alignment( align, p_d );

    document out{ };
    picture  p1{ OPT::YSCALE( ".75" ) };
    {
        place_string( p1, pname_s.slice( p_fragment ), tikz_point{ 0.0, 1.5 * CHAR_HEIGHT } );

        picture scope{ OPT::TRANSFORM_SHAPE, {} };
        for( u64 i = 0; i < p_fragment.open_end( ); ++i ) {
            auto frag = p_fragment.d_slice( p_d, i );

            std::string label = std::string{ "P_{" } + std::to_string( p_d ) + ","
                                + std::to_string( i ) + "} = P\\fragmentco{ p_{"
                                + std::to_string( i ) + "}^{<" + std::to_string( p_d ) + "}}{p_{"
                                + std::to_string( i + 1 ) + "}^{>" + std::to_string( p_d )
                                + "}} = P\\fragment{" + std::to_string( frag.closed_begin( ) )
                                + "}{" + std::to_string( frag.closed_end( ) ) + "}";

            auto pos = tikz_point{ frag.closed_begin( ) * CHAR_WIDTH, i * -1.19 * CHAR_HEIGHT };

            auto str = stylized_string{ label, frag, str_displ_t::NAME };
            str.highlight_position( i - frag.closed_begin( ), SEP_COL.deemphasize_weak( ) );
            place_string( scope, str, pos );
        }
        p1.add_scope( scope );
    }
    out.add_picture( p1 );

    // slice [5..6]
    auto    merged = graph_slice::merged_slice( gs, fragmentco{ 5, 7 } );
    picture p2{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices( p2, { merged }, p_alignment, p_P, p_T, COLOR_NONE,
                            COLOR_BLACK.deemphasize( ), COLOR_NONE, COLOR_NONE );
        place_graph_slices_t_labels( p2, { merged }, p_Tname, p_Pname, true, .5 );
        place_graph_slices_p_labels( p2, { merged }, p_d, false, .5 );
    }
    out.add_picture( p2 );
    picture p3{ OPT::YSCALE( ".9" ) };
    {
        place_graph_slices_stylized( p3, { merged }, p_alignment, COLOR_BLACK.deemphasize( ),
                                     COLOR_NONE, COLOR_NONE, COLOR_NONE, MAT_COL.deemphasize( ),
                                     COLOR_BLACK.deemphasize( ) );
        place_graph_slices_t_labels( p3, { merged }, p_Tname, p_Pname, true, .5 );
        place_graph_slices_p_labels( p3, { merged }, p_d, false, .5 );
    }
    out.add_picture( p3 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

int main( int p_argc, char* p_argv[] ) {
    PROGRAM_NAME = p_argv[ 0 ];
    if( p_argc > 1 ) { OUT_DIR = std::string{ p_argv[ 1 ] }; }
    if( p_argc > 2 ) {
        TEX_DIR    = std::string{ p_argv[ 2 ] };
        FONT_PATH  = TEX_DIR + "font";
        COLOR_PATH = TEX_DIR + "color";
        MACRO_PATH = TEX_DIR + "macros";
    }

    alignment_picture( );
    alignment_graph_picture( );
    slices_picture( );
    slices_picture( "g03b.tex", P_NAME, P_NAME, fragmentco{ 0, 11 }, 1,
                    trivial_alignment( fragmentco{ 0, 11 } ), P, P );
    slices_picture( "g03c.tex", P_NAME, S_NAME, fragmentco{ 0, 20 }, 1, BP_P2_T2, P2, T2 );
    slices_detail_picture( );
}
