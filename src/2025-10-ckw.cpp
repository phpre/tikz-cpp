#include <cstdio>
#include <string>

#include "breakpoint.h"
#include "tikz.h"
#include "tikz_graph.h"
#include "tikz_string.h"
using namespace TIKZ;

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

std::string OUT_DIR        = "../figs/";
std::string TEX_DIR        = "../tex/";
std::string FONT_PATH      = TEX_DIR + "font";
std::string COLOR_PATH     = TEX_DIR + "color";
std::string MACRO_PATH     = TEX_DIR + "macros";
std::string PACKAGES       = "";
std::string LIBRARIES      = "";
std::string EXTRA_PREAMBLE = "";
std::string PROGRAM_NAME   = "";

#define NEW_DOC_SIMPLE( p_name )                                                                \
    new_document( OUT_DIR + ( p_name ), FONT_PATH, COLOR_PATH, MACRO_PATH, PACKAGES, LIBRARIES, \
                  EXTRA_PREAMBLE );

void alignment_picture( const std::string& p_name      = "g01.tex",
                        breakpoint_repn    p_alignment = BP_P_T ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );
    initialize_tikzpicture( out );

    print_alignment( out, P_NAME2, tikz_point{ 0.0, 0.0 }, T_NAME2, tikz_point{ 0.0, 1.25 },
                     p_alignment, true );
    finish_tikzpicture( out );

    initialize_tikzpicture( out );
    auto next
        = print_alignment( out, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 },
                           breakpoint_slice( p_alignment, { 0, 6 } ), true );

    next = translate_right( next, 2.5 );
    next = print_alignment( out, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 6, 10 } ), true );
    finish_tikzpicture( out );

    initialize_tikzpicture( out );
    next = translate_right( next, -next.first.m_x );
    next = print_alignment( out, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 13 } ), true );
    next = translate_right( next, 2.5 );
    next = print_alignment( out, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 13, 22 } ), true );
    finish_tikzpicture( out );

    initialize_tikzpicture( out );
    next = print_alignment( out, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 },
                            breakpoint_slice( p_alignment, { 0, 1 } ), true );

    next = translate_right( next, 1 );
    next = print_alignment( out, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 10 } ), true );

    next = translate_right( next, 1 );
    next = print_alignment( out, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 11 } ), true );

    next = translate_right( next, 1 );
    next = print_alignment( out, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 11, 11 } ), true );
    finish_tikzpicture( out );
    finish_document( out );
}

void alignment_graph_picture(
    const std::string& p_name = "g02.tex",
    stylized_string    p_Tname
    = stylized_string{ T_NAME.m_name, T_NAME.m_fragment, str_displ_t::POSITION },
    stylized_string p_Pname
    = stylized_string{ P_NAME.m_name, P_NAME.m_fragment, str_displ_t::POSITION },
    fragmentco p_fragment = fragmentco{ 0, 11 }, breakpoint_repn p_alignment = BP_P_T ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );

    initialize_tikzpicture( out, "yscale = .93" );
    {
        auto fragT = align_fragment( p_alignment, p_fragment );

        print_alignment_graph_label( out, p_Pname.slice( p_fragment ), p_Tname.slice( fragT ) );
        auto vg = print_alignment_graph( out, P, p_fragment, T, fragT );
        // highlight main diagonal
        vg.print_diagonal_on_coordinates( out, vertex::marked_vertex( SEP_COL.deemphasize_weak( ) ),
                                          1 + std::min( p_fragment.length( ), fragT.length( ) ) );
        // print alignment
        auto brpnt = breakpoint_slice( p_alignment, p_fragment );
        print_alignment_on_coordinates( out, vg, brpnt );
    }
    finish_tikzpicture( out );

    initialize_tikzpicture( out, "yscale = .93" );
    {
        print_string_vertical(
            out, stylized_string{ "P\\position{p}", fragmentco{ 0, 1 }, str_displ_t::NAME },
            tikz_point{ -CHAR_WIDTH - .4, -.5 * CHAR_HEIGHT } );

        auto p_indent = 4;
        auto p_out    = out;
        INDENT_PRINT( 1 )( out, "\\begin{scope}\n" );
        INDENT_PRINT( 2 )( out, "\\clip (%5.3lf, %5.3lf) rectangle (%5.3lf, %5.3lf);\n",
                           -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT, .67 * CHAR_WIDTH,
                           -2.67 * CHAR_HEIGHT );

        auto vg = print_alignment_graph(
            out, "PPP", { 0, 3 }, "TTT", { 0, 2 }, tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
            tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH }, COLOR_TEXT, 2 );

        print_selected_arrow( out, vg.label_for_pos( 1, 1 ), vg.label_for_pos( 1, 2 ), DEL_COL,
                              DEL_COL.to_bg( ), -90.0, 2 );
        INDENT_PRINT( 1 )( out, "\\end{scope}\n" );

        print_text( out, math_mode( "\\small w( P\\position{p} \\to \\varepsilon )" ),
                    tikz_point{ 2 * CHAR_WIDTH, -1 * CHAR_HEIGHT },
                    "fill = white, xscale=.8, inner sep = 1pt, ", DEL_COL );
    }
    finish_tikzpicture( out );

    initialize_tikzpicture( out, "yscale = .93" );
    {
        print_string( out,
                      stylized_string{ "T\\position{t}", fragmentco{ 0, 1 }, str_displ_t::NAME },
                      tikz_point{ .5 * CHAR_WIDTH, CHAR_HEIGHT + .4 } );

        auto p_indent = 4;
        auto p_out    = out;
        INDENT_PRINT( 1 )( out, "\\begin{scope}\n" );
        INDENT_PRINT( 2 )( out, "\\clip (%5.3lf, %5.3lf) rectangle (%5.3lf, %5.3lf);\n",
                           -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT, 2.67 * CHAR_WIDTH,
                           -.67 * CHAR_HEIGHT );

        auto vg = print_alignment_graph(
            out, "PPP", { 0, 2 }, "TTT", { 0, 3 }, tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
            tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH }, COLOR_TEXT, 2 );

        print_selected_arrow( out, vg.label_for_pos( 1, 1 ), vg.label_for_pos( 2, 1 ), INS_COL,
                              INS_COL.to_bg( ), -0.0, 2 );
        INDENT_PRINT( 1 )( out, "\\end{scope}\n" );

        print_text( out, math_mode( "\\small w( \\varepsilon \\to T\\position{t} )" ),
                    tikz_point{ 1 * CHAR_WIDTH, -1.25 * CHAR_HEIGHT },
                    "fill = white, xscale=.8, inner sep = 1pt, ", INS_COL, 1 );
    }
    finish_tikzpicture( out );

    initialize_tikzpicture( out, "yscale = .93" );
    {
        print_string( out,
                      stylized_string{ "T\\position{t}", fragmentco{ 0, 1 }, str_displ_t::NAME },
                      tikz_point{ .5 * CHAR_WIDTH, CHAR_HEIGHT + .4 } );
        print_string_vertical(
            out, stylized_string{ "P\\position{p}", fragmentco{ 0, 1 }, str_displ_t::NAME },
            tikz_point{ -CHAR_WIDTH - .4, -.5 * CHAR_HEIGHT } );

        auto p_indent = 4;
        auto p_out    = out;
        INDENT_PRINT( 1 )( out, "\\begin{scope}\n" );
        INDENT_PRINT( 2 )( out, "\\clip (%5.3lf, %5.3lf) rectangle (%5.3lf, %5.3lf);\n",
                           -.67 * CHAR_WIDTH, .67 * CHAR_HEIGHT, 2.67 * CHAR_WIDTH,
                           -2.67 * CHAR_HEIGHT );

        auto vg = print_alignment_graph(
            out, "PPP", { 0, 3 }, "TTT", { 0, 3 }, tikz_point{ 2 * CHAR_WIDTH, -2 * CHAR_HEIGHT },
            tikz_point{ -2 * CHAR_WIDTH, 2 * CHAR_WIDTH }, COLOR_TEXT, 2 );

        INDENT_PRINT( 1 )( out, "\\end{scope}\n" );

        print_text( out, math_mode( "\\small w( P\\position{p} \\to T\\position{t} )" ),
                    tikz_point{ 1.3 * CHAR_WIDTH, -.7 * CHAR_HEIGHT },
                    "anchor=west, fill = white, xscale=.8, inner sep = 1pt, ", SUB_COL );

        print_text( out, math_mode( "\\small 0" ),
                    tikz_point{ .7 * CHAR_WIDTH, -1.3 * CHAR_HEIGHT },
                    "anchor=east, fill = white, xscale=.8, inner sep = 1pt, ", MAT_COL );

        print_selected_arrow( out, vg.label_for_pos( 1, 1 ) + ") + (0.0, -.05",
                              vg.label_for_pos( 2, 2 ) + ") + ( 0.0, -.05", MAT_COL,
                              MAT_COL.to_bg( ), -45.0 );

        print_selected_arrow( out, vg.label_for_pos( 1, 1 ) + ") + (0.0, +.05",
                              vg.label_for_pos( 2, 2 ) + ") + ( 0.0, +.05", SUB_COL,
                              SUB_COL.to_bg( ), -45.0 );
    }
    finish_tikzpicture( out );
    finish_document( out );
}

void slices_picture( const std::string& p_name = "g03.tex", stylized_string p_Pname = P_NAME,
                     stylized_string p_Tname = S_NAME, fragmentco p_fragment = fragmentco{ 0, 11 },
                     u32 p_d = 1, breakpoint_repn p_alignment = BP_P_T, const std::string& p_P = P,
                     const std::string& p_T = T ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );

    auto pname_s = stylized_string{ p_Pname.m_name, p_Pname.m_fragment, str_displ_t::NAME };
    auto tname_s = stylized_string{ p_Tname.m_name, p_Tname.m_fragment, str_displ_t::NAME };

    auto align = breakpoint_slice( p_alignment, p_fragment );

    auto gs = graph_slice::from_alignment( align, p_d );
    // full picture of every slice
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices( out, gs, p_alignment, p_P, p_T, COLOR_BLACK.deemphasize( ), COLOR_NONE,
                            COLOR_NONE, COLOR_NONE );
        print_graph_slices_t_labels( out, gs, p_Tname, p_Pname );
        print_graph_slices_p_labels( out, gs, p_d );
    }
    finish_tikzpicture( out );
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices_stylized( out, gs, p_alignment );
        print_graph_slices_t_labels( out, gs, p_Tname, p_Pname, true );
        print_graph_slices_p_labels( out, gs, p_d );
    }
    finish_tikzpicture( out );

    // left-pure-right decomposition
    auto tpb_decomp = graph_slice::top_pure_bot_decomp( gs );
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices( out, tpb_decomp, p_alignment, p_P, p_T, COLOR_BLACK.deemphasize( ),
                            COLOR_NONE, COLOR_NONE );
        print_graph_slices_t_labels( out, tpb_decomp, p_Tname, p_Pname, true );
        print_string_vertical( out, pname_s.slice( p_fragment ),
                               tikz_point{ -CHAR_WIDTH - .25, .0 } );
    }
    finish_tikzpicture( out );
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices_stylized( out, tpb_decomp, p_alignment );
        print_graph_slices_t_labels( out, tpb_decomp, p_Tname, p_Pname, true );
        print_string_vertical( out, pname_s.slice( p_fragment ),
                               tikz_point{ -CHAR_WIDTH - .25, .0 } );
    }
    finish_tikzpicture( out );

    // fully merged
    auto merged = graph_slice::merged_slice( gs, p_fragment );
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices( out, { merged }, p_alignment, p_P, p_T, COLOR_NONE,
                            COLOR_BLACK.deemphasize( ), COLOR_NONE, COLOR_NONE );
        print_alignment_graph_label( out, pname_s.slice( p_fragment ),
                                     tname_s.slice( merged.fragment_t( ) ) );
    }
    finish_tikzpicture( out );
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices_stylized( out, { merged }, p_alignment, COLOR_BLACK.deemphasize( ),
                                     COLOR_NONE, SEP_COL.deemphasize( ), SEP_COL.deemphasize( ),
                                     MAT_COL.deemphasize( ), COLOR_BLACK.deemphasize( ) );
        print_alignment_graph_label( out, pname_s.slice( p_fragment ),
                                     tname_s.slice( merged.fragment_t( ) ) );
    }
    finish_tikzpicture( out );
    finish_document( out );
}

void slices_detail_picture( const std::string& p_name = "g04.tex", stylized_string p_Pname = P_NAME,
                            stylized_string p_Tname = S_NAME,
                            fragmentco p_fragment = fragmentco{ 0, 11 }, u32 p_d = 1,
                            breakpoint_repn p_alignment = BP_P_T, const std::string& p_P = P,
                            const std::string& p_T = T ) {
    FILE* out     = NEW_DOC_SIMPLE( p_name );
    auto  pname_s = stylized_string{ p_Pname.m_name, p_Pname.m_fragment, str_displ_t::NAME };
    auto  tname_s = stylized_string{ p_Tname.m_name, p_Tname.m_fragment, str_displ_t::NAME };

    auto align = breakpoint_slice( p_alignment, p_fragment );

    auto gs = graph_slice::from_alignment( align, p_d );

    initialize_tikzpicture( out, "yscale = .75" );
    {
        print_string( out, pname_s.slice( p_fragment ), tikz_point{ 0.0, 1.5 * CHAR_HEIGHT } );

        for( u32 i = 0; i < p_fragment.open_end( ); ++i ) {
            auto frag = p_fragment.d_slice( p_d, i );

            std::string label = std::string{ "P_{" } + std::to_string( p_d ) + ","
                                + std::to_string( i ) + "} = P\\fragmentco{ p_{"
                                + std::to_string( i ) + "}^{<" + std::to_string( p_d ) + "}}{ p_{"
                                + std::to_string( i + 1 ) + "}^{>" + std::to_string( p_d )
                                + "}} = P\\fragment{" + std::to_string( frag.closed_begin( ) )
                                + "}{" + std::to_string( frag.closed_end( ) ) + "}";

            auto pos = tikz_point{ frag.closed_begin( ) * CHAR_WIDTH, i * -1.19 * CHAR_HEIGHT };

            auto str = stylized_string{ label, frag, str_displ_t::NAME };
            str.highlight_position( i - frag.closed_begin( ), SEP_COL.deemphasize_weak( ) );
            print_string( out, str, pos );
        }
    }
    finish_tikzpicture( out );

    // slice [5..6]
    auto merged = graph_slice::merged_slice( gs, fragmentco{ 5, 7 } );
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices( out, { merged }, p_alignment, p_P, p_T, COLOR_NONE,
                            COLOR_BLACK.deemphasize( ), COLOR_NONE, COLOR_NONE );
        print_graph_slices_t_labels( out, { merged }, p_Tname, p_Pname, true, .5 );
        print_graph_slices_p_labels( out, { merged }, p_d, false, .5 );
    }
    finish_tikzpicture( out );
    initialize_tikzpicture( out, "yscale = .9" );
    {
        print_graph_slices_stylized( out, { merged }, p_alignment, COLOR_BLACK.deemphasize( ),
                                     COLOR_NONE, COLOR_NONE, COLOR_NONE, MAT_COL.deemphasize( ),
                                     COLOR_BLACK.deemphasize( ) );
        print_graph_slices_t_labels( out, { merged }, p_Tname, p_Pname, true, .5 );
        print_graph_slices_p_labels( out, { merged }, p_d, false, .5 );
    }
    finish_tikzpicture( out );

    finish_document( out );
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
