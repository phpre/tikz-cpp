#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font_default";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      globals
//
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures; one file per frame
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, {
    constexpr std::string T = "abbbabaabbbababbb";
    constexpr std::string P = "ababbabaabb";

    auto bps    = compute_occs_with_edits( P, T, 3 );
    auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD };
    auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

    breakpoint_repn res{ };
    res.push_back( breakpoint{ 0, 0 } );
    res.push_back( breakpoint{ 2, 2, 'a' } );
    res.push_back( breakpoint{ 9, 8, 0, 'b' } );
    res.push_back( breakpoint{ 11, 11 } );
    bps.push_back( res );

    add_occurrences_pictures( doc, P_NAME, T_NAME, { res, bps[ 4 ] }, occ_style_t::CHAR_POS );

    bps.clear( );
    {
        breakpoint_repn res{ };
        res.push_back( breakpoint{ 3, 5 } );
        res.push_back( breakpoint{ 4, 6, 'b' } );
        res.push_back( breakpoint{ 6, 7 } );
        bps.push_back( res );
    }
    add_occurrences_pictures( doc, P_NAME.slice( fragmentco{ 3, 6 } ), T_NAME, bps,
                              occ_style_t::CHAR_POS );

    /*
        for( u64 y = 0; y < bps.size( ); ++y ) {
            WITH_PICTURE( p3, { }, doc ) {
                place_alignment( p3, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.0 },
                                 bps[ y ], AT_PRINT_EXTRA_STRING_PARTS | AT_COMPRESS );
            }
        }
        */
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
