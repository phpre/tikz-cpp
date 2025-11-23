#include <cstdio>
#include "tikz.h"
using namespace TIKZ;

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

    document out{ };
    picture  p1{ };
    for( u64 j = 1, i2 = 0; j < str_displ_t::MAX; j <<= 1, ++i2 ) {
        p1.place_text( VSIZE_CORRECTION + optname[ i2 ],
                       tikz_point{ ( data.length( ) + 1 + i2 ) * CHAR_WIDTH, CHAR_HEIGHT * ( .5 ) },
                       OPT::ROTATE( "90" ) | OPT::ANCHOR_WEST );
    }
    for( u64 i = 0; i < str_displ_t::MAX; i += 2 ) {
        auto S = stylized_string{ data, name, i }.add_wildcards( std::deque<u64>{ 0, 1, 5 }, true );
        auto SI = S.color_invert( );
        place_string( p1, S, tikz_point{ .0, -CHAR_HEIGHT * 1.0 * i } );
        place_string( p1, SI,
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
    out.add_picture( p1 );
    picture p2{ };
    for( u64 i = 0; i < str_displ_t::MAX / 2; ++i ) {
        auto S = stylized_string{ data, name, i }.add_wildcards( std::deque<u64>{ 0, 1, 5 }, true );
        auto SI = S.color_invert( );
        place_string_vertical( p2, S, tikz_point{ CHAR_WIDTH * 2.0 * i, .0 } );
        place_string_vertical(
            p2, SI, tikz_point{ CHAR_WIDTH * ( 2.0 * i ), ( data.length( ) + 1 ) * CHAR_WIDTH } );
    }
    out.add_picture( p2 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_alignment( const std::string& p_name = "g02.tex" ) {
    document out{ };
    picture  p1{ };
    {
        std::string T = "abab" + WILDCARD + "abcaab" + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "babcabcaccabaab";

        auto T_NAME
            = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards( T, true );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bp = compute_breakpoints( P, T, WILDCARD );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 }, bp );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 3.5 }, T_NAME, tikz_point{ 0.0, 4.75 }, bp,
                         false, false, true );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 6.5 }, T_NAME, tikz_point{ 0.0, 7.75 }, bp,
                         false );
    }
    out.add_picture( p1 );
    picture p2{ };
    {
        std::string T = "b" + WILDCARD + WILDCARD + WILDCARD + "a" + WILDCARD + "ab" + WILDCARD
                        + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "bcaaa";

        auto T_NAME
            = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards( T, true );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs( P, T, WILDCARD );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            place_alignment( p2, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ], false, true, true );

            place_alignment( p2, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], true, true );
        }
    }
    out.add_picture( p2 );
    picture p3{ };
    {
        std::string T = "aabaabcaabaabba";
        std::string P = "bcaaa";

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD };
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs_with_edits( P, T, 2 );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            place_alignment( p3, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ], false, true, true );

            place_alignment( p3, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], true, true );
        }
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

    picture_string( );
    picture_alignment( );
}
