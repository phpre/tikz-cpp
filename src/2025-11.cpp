#include <cstdio>
#include <string>

#include "tikz.h"
#include "tikz_string.h"
#include "wildcard.h"

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

std::string T = "abab" + WILDCARD + "abcaab" + WILDCARD + "a" + WILDCARD + "ab";
std::string P = "abcab";

const stylized_string T_NAME{ T, std::string{ "T" },
                              str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
const stylized_string T_WC = T_NAME.add_wildcards( T, true );
const stylized_string P_NAME{ P, std::string{ "P" }, str_displ_t::FRAGMENT };

void wildcards_picture( const std::string& p_name = "g01.tex" ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );
    initialize_tikzpicture( out );

    print_string( out, T_WC.slice( fragmentco{ 0, static_cast<u32>( T.size( ) ) } ),
                  tikz_point{ 0, 0 } );

    finish_tikzpicture( out );
    finish_document( out );
}

void trie_picture( const std::string& p_name = "g02.tex" ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );
    initialize_tikzpicture( out );

    finish_tikzpicture( out );
    finish_document( out );
}

void multi_trie_picture( const std::string& p_name = "g02.tex" ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );
    initialize_tikzpicture( out );

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

    wildcards_picture( );
    // trie_picture( );
    // multi_trie_picture( );
}
