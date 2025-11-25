#include "tikz.h"
using namespace TIKZ;

std::string OUT_DIR      = "../figs/";
std::string TEX_DIR      = "../tex/";
std::string FONT_PATH    = TEX_DIR + "font";
std::string COLOR_PATH   = TEX_DIR + "color";
std::string MACRO_PATH   = TEX_DIR + "macros";
std::string PROGRAM_NAME = "";

std::string T = "b" + WILDCARD + WILDCARD + WILDCARD + "a" + WILDCARD + "ab" + WILDCARD + WILDCARD
                + "a" + WILDCARD + "a";
std::string P = "bcaaa";

auto T_NAME = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS }
                  .add_wildcards( T, true );
auto P_NAME = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

auto OCCS_P_T     = compute_occs( P, T, WILDCARD );
auto OCCS_P_T_ALL = compute_occs_with_mism( P, T, P.size( ), WILDCARD );

void wildcards_picture( const std::string& p_name = "g01.tex" ) {
    document out{ };
    for( u64 y = 0; y < OCCS_P_T.size( ); ++y ) {
        picture p2{ };
        place_alignment( p2, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                         tikz_point{ 0.0, -3.0 * y + 1 }, OCCS_P_T[ y ], false, true, true, true );
        out.add_picture( p2 );
    }
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void wildcards_picture2( const std::string& p_name = "g01b.tex" ) {
    document out{ };
    for( u64 y = 0; y < OCCS_P_T_ALL.size( ); ++y ) {
        picture p2{ };
        place_alignment( p2, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                         tikz_point{ 0.0, -3.0 * y + 1 }, OCCS_P_T_ALL[ y ], false, true, true,
                         true );
        out.add_picture( p2 );
    }
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void trie_picture( const std::string& p_name = "g02.tex" ) {
    trie                    T;
    std::deque<std::string> str{ "abab", "b", "", "bba", "cba", "cc", "aaab", "aa" };
    // std::deque<std::string> str{ "aaa", "aab" };

    document out{ };
    for( const auto& s : str ) {
        picture p1{ };
        T.insert( s );
        place_trie_depth_labels( p1, T, tikz_point{ 0, CHAR_HEIGHT * 3 / 4 } );
        auto tr = place_trie( p1, T );
        place_trie_string_on_coordinates( p1, tr, s, OPT::COLOR( COLOR_C3 ) );
        out.add_picture( p1 );
    }
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void multi_trie_picture( const std::string& p_name = "g03.tex" ) {
    std::deque<std::string> str{ "caa", "bcaa", "*bca", "****caa", "****bca" };
    document                out{ };
    picture                 p1{ };
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
    wildcards_picture2( );
    trie_picture( );
    // multi_trie_picture( );
}
