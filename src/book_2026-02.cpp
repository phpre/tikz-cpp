#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( s00_p01, {
    WITH_PICTURE( p1, { }, doc ) {
        auto tn = stylized_string{ "T", fragmentco{ 0, 5 }, str_displ_t::NAME };
        place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    }

    constexpr std::string T2       = "DEFGHaJKLMcOPQAB";
    constexpr std::string P2       = "DEFGHIJKLMNOPQAB";
    const breakpoint_repn BP_P2_T2 = compute_breakpoints( P2, T2 );

    const stylized_string T2_NAME{ T2, std::string{ "T" }, str_displ_t::FRAGMENT };
    const stylized_string P2_NAME{ P2, std::string{ "P" }, str_displ_t::FRAGMENT };

    add_alignment_picture( doc, BP_P2_T2, T2_NAME, P2_NAME, AT_COMPRESS );
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } );
