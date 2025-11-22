#include <cstdio>
#include <string>

#include "tikz.h"
#include "tikz_document.h"
#include "tikz_picture.h"
#include "tikz_string.h"

std::string OUT_DIR      = "../figs/";
std::string TEX_DIR      = "../tex/";
std::string FONT_PATH    = TEX_DIR + "font";
std::string COLOR_PATH   = TEX_DIR + "color";
std::string MACRO_PATH   = TEX_DIR + "macros";
std::string PROGRAM_NAME = "";

void picture_alignment( const std::string& p_name = "g01.tex" ) {
    TIKZ::document out{ };
    TIKZ::picture  p1{ };
    {
        std::string T = "abab" + WILDCARD + "abcaab" + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "babcabcaccabaab";

        auto T_NAME
            = TIKZ::stylized_string{ T, "T", TIKZ::str_displ_t::FRAGMENT_WILDCARD }.add_wildcards(
                T, true );
        auto P_NAME = TIKZ::stylized_string{ P, "P", TIKZ::str_displ_t::FRAGMENT_WILDCARD };

        auto bp = compute_breakpoints( P, T, WILDCARD );

        TIKZ::place_alignment( p1, P_NAME, TIKZ::tikz_point{ 0.0, 0.0 }, T_NAME,
                               TIKZ::tikz_point{ 0.0, 1.25 }, bp );

        TIKZ::place_alignment( p1, P_NAME, TIKZ::tikz_point{ 0.0, 3.5 }, T_NAME,
                               TIKZ::tikz_point{ 0.0, 4.75 }, bp, false, false, true );

        TIKZ::place_alignment( p1, P_NAME, TIKZ::tikz_point{ 0.0, 6.5 }, T_NAME,
                               TIKZ::tikz_point{ 0.0, 7.75 }, bp, false );
    }
    out.add_picture( p1 );
    TIKZ::picture p2{ };
    {
        std::string T = "b" + WILDCARD + WILDCARD + WILDCARD + "a" + WILDCARD + "ab" + WILDCARD
                        + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "bcaaa";

        auto T_NAME
            = TIKZ::stylized_string{ T, "T", TIKZ::str_displ_t::FRAGMENT_WILDCARD }.add_wildcards(
                T, true );
        auto P_NAME = TIKZ::stylized_string{ P, "P", TIKZ::str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs( P, T, WILDCARD );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            TIKZ::place_alignment( p2, P_NAME, TIKZ::tikz_point{ 0.0, -3.0 * y }, T_NAME,
                                   TIKZ::tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ], false, true,
                                   true );

            TIKZ::place_alignment(
                p2, P_NAME, TIKZ::tikz_point{ ( 2 + T.length( ) ) * TIKZ::CHAR_WIDTH, -3.0 * y },
                T_NAME, TIKZ::tikz_point{ ( 2 + T.length( ) ) * TIKZ::CHAR_WIDTH, -3.0 * y + 1 },
                bps[ y ], true, true );
        }
    }
    out.add_picture( p2 );
    TIKZ::picture p3{ };
    {
        std::string T = "aabaabcaabaabba";
        std::string P = "bcaaa";

        auto T_NAME = TIKZ::stylized_string{ T, "T", TIKZ::str_displ_t::FRAGMENT_WILDCARD };
        auto P_NAME = TIKZ::stylized_string{ P, "P", TIKZ::str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs_with_edits( P, T, 2 );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            TIKZ::place_alignment( p3, P_NAME, TIKZ::tikz_point{ 0.0, -3.0 * y }, T_NAME,
                                   TIKZ::tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ], false, true,
                                   true );

            TIKZ::place_alignment(
                p3, P_NAME, TIKZ::tikz_point{ ( 2 + T.length( ) ) * TIKZ::CHAR_WIDTH, -3.0 * y },
                T_NAME, TIKZ::tikz_point{ ( 2 + T.length( ) ) * TIKZ::CHAR_WIDTH, -3.0 * y + 1 },
                bps[ y ], true, true );
        }
    }
    out.add_picture( p3 );
    TIKZ::document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
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

    picture_alignment( );
}
