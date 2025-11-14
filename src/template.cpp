#include <cstdio>
#include <string>

#include "tikz.h"
#include "tikz_string.h"

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
                  EXTRA_PREAMBLE );                                                             \
    fprintf( stderr, "[%s] Generating %s.\n", PROGRAM_NAME.c_str( ),                            \
             ( OUT_DIR + ( p_name ) ).c_str( ) );

void picture_stub( const std::string& p_name = "g01.tex" ) {
    FILE* out = NEW_DOC_SIMPLE( p_name );
    initialize_tikzpicture( out );

    std::string longstr  = "hello world!";
    std::string shortstr = "S";

    for( u32 i = 0; i < 8; ++i ) {
        auto S = stylized_string{ i ? shortstr : longstr, fragmentco{ 0, longstr.size( ) },
                                  static_cast<str_displ_t>( i ) }
                     .add_wildcards( WILDCARD + WILDCARD + "llo" + WILDCARD + "world!", true );
        auto SI = S.color_invert( );
        print_string( out, S, tikz_point{ .0, -CHAR_HEIGHT * 4.0 * i } );
        print_string( out, SI, tikz_point{ .0, -CHAR_HEIGHT * ( 4.0 * i + 2 ) } );
    }

    finish_tikzpicture( out );
    initialize_tikzpicture( out );

    for( u32 i = 0; i < 8; ++i ) {
        auto S = stylized_string{ i ? shortstr : longstr, fragmentco{ 0, longstr.size( ) },
                                  static_cast<str_displ_t>( i ) }
                     .add_wildcards( WILDCARD + WILDCARD + "llo" + WILDCARD + "world!", true );
        auto SI = S.color_invert( );
        print_string_vertical( out, S, tikz_point{ CHAR_WIDTH * 4.0 * i, .0 } );
        print_string_vertical( out, SI, tikz_point{ CHAR_WIDTH * ( 4.0 * i + 2 ), .0 } );
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

    picture_stub( );
}
