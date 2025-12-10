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

void add_string_demo_pictures( document& p_doc ) {
    std::deque<std::string> optname{ "rotatable",      "show positions",  "show characters",
                                     "show wildcards", "group positions", "use typewriter" };
    std::string             data = "hello world!";
    std::string             name = "S";

    WITH_PICTURE( p1, { }, p_doc ) {
        for( u64 j = 1, i2 = 0; j < str_displ_t::MAX; j <<= 1, ++i2 ) {
            p1.place_text(
                VSIZE_CORRECTION + optname[ i2 ],
                tikz_point{ ( data.length( ) + 1 + i2 ) * CHAR_WIDTH, CHAR_HEIGHT * ( .5 ) },
                OPT::ROTATE( "90" ) | OPT::ANCHOR_WEST );
        }
        for( u64 i = 0; i < str_displ_t::MAX; i += 2 ) {
            auto S = stylized_string{ data, name, i }.add_wildcards(
                std::deque<u64>{ 0, 1, 5 }, chr_displ_t::SHOW_ID_IF_WILDCARD );
            auto SI = S.color_invert( );
            place_string( p1, S, tikz_point{ .0, -CHAR_HEIGHT * 1.0 * i } );
            place_string(
                p1, SI,
                tikz_point{ ( data.length( ) + 1 ) * -CHAR_WIDTH, -CHAR_HEIGHT * 1.0 * i } );
            for( u64 j = 1, i2 = 0; j < str_displ_t::MAX; j <<= 1, ++i2 ) {
                tikz_point pos{ ( data.length( ) + 1 + i2 ) * CHAR_WIDTH,
                                -CHAR_HEIGHT * ( 1.0 * i + .5 ) };
                if( i & j ) {
                    p1.place_maru( pos );
                } else {
                    p1.place_batsu( pos );
                }
            }
        }
    }
    WITH_PICTURE( p2, { }, p_doc ) {
        for( u64 i = 0; i < str_displ_t::MAX / 2; ++i ) {
            auto S = stylized_string{ data, name, i }.add_wildcards(
                std::deque<u64>{ 0, 1, 5 }, chr_displ_t::SHOW_ID_IF_WILDCARD );
            auto SI = S.color_invert( );
            place_string_vertical( p2, S, tikz_point{ CHAR_WIDTH * 2.0 * i, .0 } );
            place_string_vertical(
                p2, SI,
                tikz_point{ CHAR_WIDTH * ( 2.0 * i ), ( data.length( ) + 1 ) * CHAR_WIDTH } );
        }
    }
}

void add_alignment_demo_pictures( document& p_doc ) {
    WITH_PICTURE( p1, { }, p_doc ) {
        std::string T = "abab" + WILDCARD + "abcaab" + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "babcabcaccabaab";

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards(
            T, chr_displ_t::SHOW_ID_IF_WILDCARD );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bp = compute_breakpoints( P, T, WILDCARD );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 }, bp );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 3.5 }, T_NAME, tikz_point{ 0.0, 4.75 }, bp,
                         AT_COMPRESS );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 6.5 }, T_NAME, tikz_point{ 0.0, 7.75 }, bp,
                         AT_NONE );
    }
    WITH_PICTURE( p2, { }, p_doc ) {
        std::string T = "b" + WILDCARD + WILDCARD + WILDCARD + "a" + WILDCARD + "ab" + WILDCARD
                        + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "bcaaa";

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards(
            T, chr_displ_t::SHOW_ID_IF_WILDCARD );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs( P, T, WILDCARD );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            place_alignment( p2, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ],
                             AT_PRINT_EXTRA_STRING_PARTS | AT_COMPRESS );

            place_alignment( p2, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], AT_PRINT_BREAKPOINTS | AT_PRINT_EXTRA_STRING_PARTS );
        }
    }
    WITH_PICTURE( p3, { }, p_doc ) {
        std::string T = "aabaabcaabaabba";
        std::string P = "bcaaa";

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD };
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs_with_edits( P, T, 2 );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            place_alignment( p3, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ],
                             AT_PRINT_EXTRA_STRING_PARTS | AT_COMPRESS );

            place_alignment( p3, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], AT_PRINT_EXTRA_STRING_PARTS | AT_PRINT_BREAKPOINTS );
        }
    }
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, { add_string_demo_pictures( doc ); } )
FILE_SIMPLE( g02, { add_alignment_demo_pictures( doc ); } )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } );
