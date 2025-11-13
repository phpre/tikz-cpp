#include <cstdio>
#include <string>

#include "tikz.h"

std::string OUT_DIR        = "../figs/";
std::string TEX_DIR        = "../tex/";
std::string FONT_PATH      = TEX_DIR + "font";
std::string COLOR_PATH     = TEX_DIR + "color";
std::string MACRO_PATH     = TEX_DIR + "macros";
std::string PACKAGES       = "";
std::string LIBRARIES      = "";
std::string EXTRA_PREAMBLE = "";

#define NEW_DOC_SIMPLE( p_name )                                                                \
    new_document( OUT_DIR + ( p_name ), FONT_PATH, COLOR_PATH, MACRO_PATH, PACKAGES, LIBRARIES, \
                  EXTRA_PREAMBLE );                                                             \
    fprintf( stderr, "%s\n", ( OUT_DIR + ( p_name ) ).c_str( ) );

void picture_stub( const std::string& p_name = "g01.tex" ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );
    initialize_tikzpicture( out );

    finish_tikzpicture( out );
    initialize_tikzpicture( out );

    finish_tikzpicture( out );
    finish_document( out );
}

int main( int p_argc, char* p_argv[] ) {
    if( p_argc > 1 ) { OUT_DIR = std::string{ p_argv[ 1 ] }; }
    if( p_argc > 2 ) {
        TEX_DIR    = std::string{ p_argv[ 2 ] };
        FONT_PATH  = TEX_DIR + "font";
        COLOR_PATH = TEX_DIR + "color";
        MACRO_PATH = TEX_DIR + "macros";
    }

    // picture_stub( );
}
