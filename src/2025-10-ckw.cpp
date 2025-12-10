#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      globals
//
// ---------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

void add_alignment_slice_pictures( document& p_doc, breakpoint_repn p_alignment = BP_P_T ) {
    picture p2{ };
    auto    next
        = place_alignment( p2, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 },
                           breakpoint_slice( p_alignment, { 0, 6 } ), AT_PRINT_BREAKPOINTS );

    next = translate_right( next, 2.5 );
    next = place_alignment( p2, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 6, 10 } ), AT_PRINT_BREAKPOINTS );
    p_doc.add_picture( p2 );

    picture p3{ };
    next = translate_right( next, -next.first.m_x );
    next = place_alignment( p3, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 13 } ), AT_PRINT_BREAKPOINTS );
    next = translate_right( next, 2.5 );
    next = place_alignment( p3, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 13, 22 } ), AT_PRINT_BREAKPOINTS );
    p_doc.add_picture( p3 );

    picture p4{ };
    next = place_alignment( p4, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 },
                            breakpoint_slice( p_alignment, { 0, 1 } ), AT_PRINT_BREAKPOINTS );

    next = translate_right( next, 1 );
    next = place_alignment( p4, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 10 } ), AT_PRINT_BREAKPOINTS );

    next = translate_right( next, 1 );
    next = place_alignment( p4, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 10, 11 } ), AT_PRINT_BREAKPOINTS );

    next = translate_right( next, 1 );
    next = place_alignment( p4, P_NAME, next.first, T_NAME, next.second,
                            breakpoint_slice( p_alignment, { 11, 11 } ), AT_PRINT_BREAKPOINTS );
    p_doc.add_picture( p4 );
}

void add_slices_detail_pictures( document& p_doc, stylized_string p_Pname = P_NAME,
                                 stylized_string p_Tname = S_NAME,
                                 fragmentco p_fragment = fragmentco{ 0, 11 }, u64 p_d = 1,
                                 breakpoint_repn p_alignment = BP_P_T, const std::string& p_P = P,
                                 const std::string& p_T = T ) {
    auto pname_s = stylized_string{ p_Pname.m_name, p_Pname.m_fragment, str_displ_t ::NAME };
    auto tname_s = stylized_string{ p_Tname.m_name, p_Tname.m_fragment, str_displ_t ::NAME };

    auto align = breakpoint_slice( p_alignment, p_fragment );
    auto gs    = graph_slice::from_alignment( align, p_d );

    WITH_PICTURE( p1, { OPT::YSCALE( ".75" ) }, p_doc ) {
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

    // slice [5..6]
    auto merged = graph_slice::merged_slice( gs, fragmentco{ 5, 7 } );
    WITH_PICTURE( p2, { OPT::YSCALE( ".9" ) }, p_doc ) {
        place_graph_slices( p2, { merged }, p_alignment, p_P, p_T, COLOR_NONE,
                            COLOR_BLACK.deemphasize( ), COLOR_NONE, COLOR_NONE );
        place_graph_slices_t_labels( p2, { merged }, p_Tname, p_Pname, true, .5 );
        place_graph_slices_p_labels( p2, { merged }, p_d, false, .5 );
    }
    WITH_PICTURE( p3, { OPT::YSCALE( ".9" ) }, p_doc ) {
        place_graph_slices_stylized( p3, { merged }, p_alignment, COLOR_BLACK.deemphasize( ),
                                     COLOR_NONE, COLOR_NONE, COLOR_NONE, MAT_COL.deemphasize( ),
                                     COLOR_BLACK.deemphasize( ) );
        place_graph_slices_t_labels( p3, { merged }, p_Tname, p_Pname, true, .5 );
        place_graph_slices_p_labels( p3, { merged }, p_d, false, .5 );
    }
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, {
    add_alignment_picture( doc, BP_P_T, T_NAME2, P_NAME2, AT_PRINT_BREAKPOINTS );
    add_alignment_slice_pictures( doc, BP_P_T );
} )

FILE_SIMPLE( g02, {
    add_alignment_graph_picture(
        doc, T, stylized_string{ T_NAME.m_name, T_NAME.m_fragment, str_displ_t ::POSITION }, P,
        stylized_string{ P_NAME.m_name, P_NAME.m_fragment, str_displ_t ::POSITION },
        fragmentco{ 0, 11 }, BP_P_T, true );
    add_alignment_graph_explanation_pictures( doc );
} )

FILE_SIMPLE( g03, {
    add_slices_pictures( doc, P_NAME, S_NAME, fragmentco{ 0, 11 }, 1, BP_P_T, P, T, false );
    add_slices_pictures( doc, P_NAME, S_NAME, fragmentco{ 0, 11 }, 1,
                         trivial_alignment( fragmentco{ 0, 11 } ), P, P, false );
    add_slices_pictures( doc, P_NAME, S_NAME, fragmentco{ 0, 11 }, 1, BP_P2_T2, P2, T2, false );
} )

FILE_SIMPLE( g04, { add_slices_detail_pictures( doc ); } );

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
