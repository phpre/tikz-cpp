#include <cstdio>
#include "tikz.h"
using namespace TIKZ;

std::string OUT_DIR      = "../figs/";
std::string TEX_DIR      = "../tex/";
std::string FONT_PATH    = TEX_DIR + "font";
std::string COLOR_PATH   = TEX_DIR + "color";
std::string MACRO_PATH   = TEX_DIR + "macros";
std::string PROGRAM_NAME = "";

std::string T = "abab" + WILDCARD + "abcaab" + WILDCARD + "a" + WILDCARD + "ab";
std::string P = "abcab";

const stylized_string T_NAME{ T, std::string{ "T" },
                              str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
const stylized_string T_WC = T_NAME.add_wildcards( T, true );
const stylized_string P_NAME{ P, std::string{ "P" }, str_displ_t::FRAGMENT };

void wildcards_picture( const std::string& p_name = "g01.tex" ) {
    document out{ };
    picture  p1{ };

    place_string( p1, T_WC.slice( fragmentco{ 0, T.size( ) } ), tikz_point{ 0, 0 } );

    out.add_picture( p1 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void trie_picture( const std::string& p_name = "g02.tex" ) {
    trie                    T;
    std::deque<std::string> str{ "abab", "b", "", "bba", "aaab", "aa" };
    // std::deque<std::string> str{ "aaa", "aab" };

    document out{ };
    for( const auto& s : str ) {
        picture p1{ };
        T.insert( s );
        place_trie( p1, T, tikz_point{ 0, 0 } );
        out.add_picture( p1 );
    }
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void multi_trie_picture( const std::string& p_name = "g02.tex" ) {
    document out{ };
    picture  p1{ };
    out.add_picture( p1 );
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

    wildcards_picture( );
    trie_picture( );
    // multi_trie_picture( );
}
