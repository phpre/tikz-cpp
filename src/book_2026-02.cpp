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
    constexpr std::string T2       = "ACGATCTACCG";
    constexpr std::string P2       = "ACGTGTAACCG";
    const breakpoint_repn BP_P2_T2 = compute_breakpoints( P2, T2 );

    const stylized_string T2_NAME{ T2 };
    const stylized_string P2_NAME{ P2 };

    add_alignment_picture( doc, BP_P2_T2, T2_NAME, P2_NAME );
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } );
