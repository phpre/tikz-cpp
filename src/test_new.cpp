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
    TIKZ::document out{ };
    TIKZ::picture  p1{ };

    p1.place_text( "Hello world!", TIKZ::tikz_point{ 0.0, 0.0 },
                   TIKZ::OPT::TEXT_COLOR( TIKZ::COLOR_C2 )
                       | TIKZ::OPT::FILL( TIKZ::COLOR_C1.to_bg( ) ) );
    p1.place_text( "Hello world!", TIKZ::tikz_point{ 0.0, 1.0 } );

    out.add_picture( p1 );
    TIKZ::document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );

    /*
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
    */
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
}
