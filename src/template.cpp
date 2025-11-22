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

void picture_string( const std::string& p_name = "g01.tex" ) {
    std::deque<std::string> optname{ "rotatable",      "show positions",  "show characters",
                                     "show wildcards", "group positions", "use typewriter" };
    std::string             data = "hello world!";
    std::string             name = "S";

    TIKZ::document out{ };
    TIKZ::picture  p1{ };
    for( u64 j = 1, i2 = 0; j < TIKZ::str_displ_t::MAX; j <<= 1, ++i2 ) {
        p1.place_text( TIKZ::VSIZE_CORRECTION + optname[ i2 ],
                       TIKZ::tikz_point{ ( data.length( ) + 1 + i2 ) * TIKZ::CHAR_WIDTH,
                                         TIKZ::CHAR_HEIGHT * ( .5 ) },
                       TIKZ::OPT::ROTATE( "90" ) | TIKZ::OPT::ANCHOR_WEST );
    }
    for( u64 i = 0; i < TIKZ::str_displ_t::MAX; i += 2 ) {
        auto S  = TIKZ::stylized_string{ data, name, i }.add_wildcards( std::deque<u64>{ 0, 1, 5 },
                                                                        true );
        auto SI = S.color_invert( );
        TIKZ::place_string( p1, S, TIKZ::tikz_point{ .0, -TIKZ::CHAR_HEIGHT * 1.0 * i } );
        TIKZ::place_string( p1, SI,
                            TIKZ::tikz_point{ ( data.length( ) + 1 ) * -TIKZ::CHAR_WIDTH,
                                              -TIKZ::CHAR_HEIGHT * 1.0 * i } );
        for( u64 j = 1, i2 = 0; j < TIKZ::str_displ_t::MAX; j <<= 1, ++i2 ) {
            TIKZ::tikz_point pos{ ( data.length( ) + 1 + i2 ) * TIKZ::CHAR_WIDTH,
                                  -TIKZ::CHAR_HEIGHT * ( 1.0 * i + .5 ) };
            if( i & j ) {
                p1.place_maru( pos );
            } else {
                p1.place_batsu( pos );
            }
        }
    }
    out.add_picture( p1 );
    TIKZ::picture p2{ };
    for( u64 i = 0; i < TIKZ::str_displ_t::MAX / 2; ++i ) {
        auto S  = TIKZ::stylized_string{ data, name, i }.add_wildcards( std::deque<u64>{ 0, 1, 5 },
                                                                        true );
        auto SI = S.color_invert( );
        TIKZ::place_string_vertical( p2, S, TIKZ::tikz_point{ TIKZ::CHAR_WIDTH * 2.0 * i, .0 } );
        TIKZ::place_string_vertical(
            p2, SI,
            TIKZ::tikz_point{ TIKZ::CHAR_WIDTH * ( 2.0 * i ),
                              ( data.length( ) + 1 ) * TIKZ::CHAR_WIDTH } );
    }
    out.add_picture( p2 );
    TIKZ::document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_alignment( const std::string& p_name = "g02.tex" ) {
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

    picture_string( );
    picture_alignment( );
}
