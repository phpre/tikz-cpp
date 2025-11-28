#include "tikz.h"
using namespace TIKZ;

std::string OUT_DIR      = "../figs/";
std::string TEX_DIR      = "../tex/";
std::string FONT_PATH    = TEX_DIR + "font_talk";
std::string COLOR_PATH   = TEX_DIR + "color";
std::string MACRO_PATH   = TEX_DIR + "macros";
std::string PROGRAM_NAME = "";

void picture_occurrences( const std::string& p_name, const stylized_string& p_pname,
                          const stylized_string&             p_tname,
                          const std::deque<breakpoint_repn>& p_occs ) {
    document out{ };
    for( u64 y = 0; y < p_occs.size( ); ++y ) {
        picture p2{ };

        p2.place_text( math_mode( VSIZE_CORRECTION + p_pname.m_name ),
                       tikz_point{ -1 * CHAR_WIDTH, -3 * CHAR_HEIGHT } );
        p2.place_text( math_mode( VSIZE_CORRECTION + p_tname.m_name ),
                       tikz_point{ -1 * CHAR_WIDTH, -CHAR_HEIGHT / 2 } );

        place_alignment( p2, p_pname, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, p_tname,
                         tikz_point{ 0.0, 0.0 }, p_occs[ y ], false, true, true, true );
        out.add_picture( p2 );
    }
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_structural_insight1( const std::string& p_name = "g01.tex", bool p_allowEdits = false,
                                  u64 p_n = 14, u64 p_m = 8, u64 p_k = 4 ) {
    // T aaaabbbb
    // P   aabb

    std::string T{ }, P{ };
    for( u64 i = 0; i < p_n / 2; ++i ) { T += "a"; }
    for( u64 i = 0; i < p_m / 2; ++i ) { P += "a"; }

    for( u64 i = 0; i < p_n / 2; ++i ) { T += "b"; }
    for( u64 i = 0; i < p_m / 2; ++i ) { P += "b"; }

    auto occs
        = p_allowEdits ? compute_occs_with_edits( P, T, p_k ) : compute_occs_with_mism( P, T, p_k );

    auto tn = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    for( u64 i = 0; i < p_n / 2; ++i ) { tn.annotation_at_pos( p_n / 2 + i ).m_wasWildcard = true; }
    for( u64 i = 0; i < p_m / 2; ++i ) { pn.annotation_at_pos( p_m / 2 + i ).m_wasWildcard = true; }

    picture_occurrences( p_name, pn, tn, occs );
}

void picture_structural_insight2( const std::string& p_name = "g02.tex", bool p_allowEdits = false,
                                  u64 p_n = 14, u64 p_m = 8, u64 p_k = 4, u64 p_srand = 0 ) {
    // T aabaababa
    // P   abab
    srand( p_srand );

    std::string T{ }, P{ };
    for( u64 i = 0; i < p_n; ++i ) { T += "a"; }
    for( u64 i = 0; i < p_m; ++i ) { P += "a"; }

    for( u64 i = 0; i < p_k / 2; ++i ) {
        T[ rand( ) % T.size( ) ] = 'b';
        P[ rand( ) % P.size( ) ] = 'b';
    }

    auto occs
        = p_allowEdits ? compute_occs_with_edits( P, T, p_k ) : compute_occs_with_mism( P, T, p_k );

    auto tn = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    for( u64 i = 0; i < p_n; ++i ) {
        if( T[ i ] == 'b' ) { tn.annotation_at_pos( i ).m_wasWildcard = true; }
    }
    for( u64 i = 0; i < p_m; ++i ) {
        if( P[ i ] == 'b' ) { pn.annotation_at_pos( i ).m_wasWildcard = true; }
    }

    picture_occurrences( p_name, pn, tn, occs );
}

/*
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
*/
int main( int p_argc, char* p_argv[] ) {
    PROGRAM_NAME = p_argv[ 0 ];
    if( p_argc > 1 ) { OUT_DIR = std::string{ p_argv[ 1 ] }; }
    if( p_argc > 2 ) {
        TEX_DIR    = std::string{ p_argv[ 2 ] };
        FONT_PATH  = TEX_DIR + "font_talk";
        COLOR_PATH = TEX_DIR + "color";
        MACRO_PATH = TEX_DIR + "macros";
    }

    CROSS_FILL = true;
    picture_structural_insight1( );
    picture_structural_insight1( "g01b.tex", true );
    picture_structural_insight2( );
    picture_structural_insight2( "g02b.tex", true );
}
