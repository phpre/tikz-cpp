#include <cstdio>
#include <string>

#include "tikz.h"
#include "tikz_string.h"
using namespace TIKZ;

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

void picture_string( const std::string& p_name = "g01.tex" ) {
    FILE*       out  = NEW_DOC_SIMPLE( p_name );
    std::string data = "hello world!";
    std::string name = "S";

    initialize_tikzpicture( out );
    {
        std::deque<std::string> optname{ "rotatable",      "show positions",  "show characters",
                                         "show wildcards", "group positions", "use typewriter" };

        for( u64 j = 1, i2 = 0; j < str_displ_t::MAX; j <<= 1, ++i2 ) {
            print_node(
                out, tikz_point{ ( data.length( ) + 1 + i2 ) * CHAR_WIDTH, CHAR_HEIGHT * ( .5 ) },
                optname[ i2 ], "rotate=90, anchor = west" );
        }

        for( u64 i = 0; i < str_displ_t::MAX; i += 2 ) {
            auto S  = stylized_string{ data, name, i }.add_wildcards( std::deque<u64>{ 0, 1, 5 },
                                                                      true );
            auto SI = S.color_invert( );
            print_string( out, S, tikz_point{ .0, -CHAR_HEIGHT * 1.0 * i } );
            print_string(
                out, SI,
                tikz_point{ ( data.length( ) + 1 ) * -CHAR_WIDTH, -CHAR_HEIGHT * 1.0 * i } );

            for( u64 j = 1, i2 = 0; j < str_displ_t::MAX; j <<= 1, ++i2 ) {
                print_node( out,
                            tikz_point{ ( data.length( ) + 1 + i2 ) * CHAR_WIDTH,
                                        -CHAR_HEIGHT * ( 1.0 * i + .5 ) },
                            ( i & j ) ? "o" : "x" );
            }
        }
    }
    finish_tikzpicture( out );

    initialize_tikzpicture( out );
    {

        for( u64 i = 0; i < str_displ_t::MAX / 2; ++i ) {
            auto S  = stylized_string{ data, name, i }.add_wildcards( std::deque<u64>{ 0, 1, 5 },
                                                                      true );
            auto SI = S.color_invert( );
            print_string_vertical( out, S, tikz_point{ CHAR_WIDTH * 2.0 * i, .0 } );
            print_string_vertical(
                out, SI,
                tikz_point{ CHAR_WIDTH * ( 2.0 * i ), ( data.length( ) + 1 ) * CHAR_WIDTH } );
        }
    }
    finish_tikzpicture( out );
    finish_document( out );
}

void picture_alignment( const std::string& p_name = "g02.tex" ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );
    initialize_tikzpicture( out );
    {
        std::string T = "abab" + WILDCARD + "abcaab" + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "babcabcaccabaab";

        auto T_NAME
            = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards( T, true );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bp = compute_breakpoints( P, T, WILDCARD );

        print_alignment( out, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 }, bp );

        print_alignment( out, P_NAME, tikz_point{ 0.0, 3.5 }, T_NAME, tikz_point{ 0.0, 4.75 }, bp,
                         false, false, true );

        print_alignment( out, P_NAME, tikz_point{ 0.0, 6.5 }, T_NAME, tikz_point{ 0.0, 7.75 }, bp,
                         false );
    }
    finish_tikzpicture( out );

    initialize_tikzpicture( out );
    {
        std::string T = "b" + WILDCARD + WILDCARD + WILDCARD + "a" + WILDCARD + "ab" + WILDCARD
                        + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "bcaaa";

        auto T_NAME
            = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards( T, true );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs( P, T, WILDCARD );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            print_alignment( out, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ], false, true, true );

            print_alignment( out, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], true, true );
        }
    }
    finish_tikzpicture( out );

    initialize_tikzpicture( out );
    {
        std::string T = "aabaabcaabaabba";
        std::string P = "bcaaa";

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD };
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs_with_edits( P, T, 2 );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            print_alignment( out, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ], false, true, true );

            print_alignment( out, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], true, true );
        }
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

    picture_string( );
    picture_alignment( );
}
