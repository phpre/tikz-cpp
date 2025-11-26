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

std::string SUBST_GOOD = "aabbaaa";
std::string SUBST_BAD  = "abcabca";

auto T_NAME = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS }
                  .add_wildcards( T, true );
auto P_NAME = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

auto OCCS_P_T     = compute_occs( P, T, WILDCARD );
auto OCCS_P_T_ALL = compute_occs_with_mism( P, T, P.size( ), WILDCARD );

void wildcards_picture( const std::string&                 p_name  = "g01.tex",
                        const stylized_string&             p_pname = P_NAME,
                        const stylized_string&             p_tname = T_NAME,
                        const std::deque<breakpoint_repn>& p_occs  = OCCS_P_T ) {
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

void wc_subst_picture( const std::string& p_name      = "g02.tex",
                       const std::string  p_wcSubst   = SUBST_GOOD,
                       const std::string  p_substName = "\\varphi" ) {
    document out{ };
    picture  p1{ };
    {
        u64        y = 0;
        tikz_point topLeftC1{ 0.0, 0.0 }, topLeftC2{ 5 * CHAR_WIDTH, 0.0 };

        for( u64 i = 0; i < p_wcSubst.size( ); i += 2, ++y ) {
            if( i + 1 < p_wcSubst.size( ) ) {
                // double column
                p1.place_text( math_mode( VSIZE_CORRECTION + p_substName + "(" + std::to_string( i )
                                          + ") = " ),
                               topLeftC1 + tikz_point{ 0.0, -1.5 * CHAR_HEIGHT * y },
                               OPT::INNER_SEP( ".5em" ) | OPT::ANCHOR_EAST );
                place_string( p1,
                              stylized_string{ std::string{ p_wcSubst[ i ] }, "S",
                                               str_displ_t::SHOW_CHARACTERS }
                                  .color_invert( ),
                              topLeftC1
                                  + tikz_point{ 0.0, -1.5 * CHAR_HEIGHT * y + CHAR_HEIGHT / 2 } );

                p1.place_text( math_mode( VSIZE_CORRECTION + p_substName + "("
                                          + std::to_string( i + 1 ) + ") = " ),
                               topLeftC2 + tikz_point{ 0.0, -1.5 * CHAR_HEIGHT * y },
                               OPT::INNER_SEP( ".5em" ) | OPT::ANCHOR_EAST );
                place_string( p1,
                              stylized_string{ std::string{ p_wcSubst[ i + 1 ] }, "S",
                                               str_displ_t::SHOW_CHARACTERS }
                                  .color_invert( ),
                              topLeftC2
                                  + tikz_point{ 0.0, -1.5 * CHAR_HEIGHT * y + CHAR_HEIGHT / 2 } );

            } else {
                // single column, centered
                p1.place_text( math_mode( VSIZE_CORRECTION + p_substName + "(" + std::to_string( i )
                                          + ") = " ),
                               topLeftC1
                                   + tikz_point{ ( topLeftC2.m_x - topLeftC1.m_x ) / 2.0,
                                                 -1.5 * CHAR_HEIGHT * y },
                               OPT::INNER_SEP( ".5em" ) | OPT::ANCHOR_EAST );
                place_string( p1,
                              stylized_string{ std::string{ p_wcSubst[ i ] }, "S",
                                               str_displ_t::SHOW_CHARACTERS }
                                  .color_invert( ),
                              topLeftC1
                                  + tikz_point{ ( topLeftC2.m_x - topLeftC1.m_x ) / 2.0,
                                                -1.5 * CHAR_HEIGHT * y + CHAR_HEIGHT / 2 } );
            }
        }
    }
    out.add_picture( p1 );
    picture p2{ };
    {
        tikz_point topLeftR1{ 0.0, 0.0 }, topLeftR2{ 0.0, -2.5 * CHAR_HEIGHT };
        tikz_point hdist{ 1.25 * CHAR_WIDTH, 0.0 };
        for( u64 i = 0; i < p_wcSubst.size( ); ++i ) {
            p2.place_line(
                topLeftR1 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, -CHAR_HEIGHT - .03 },
                topLeftR2 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, 0.04 },
                OPT::double_arrow( OPT::DRAW( COLOR_TEXT ) ) | OPT::DRAW( COLOR_FILL_WHITE ) );
            place_string( p2, wildcard_string( { i, i + 1 }, true ), topLeftR1 + ( hdist * i ) );
            place_string(
                p2,
                stylized_string{ std::string{ p_wcSubst[ i ] }, "S", str_displ_t::SHOW_CHARACTERS }
                    .color_invert( ),
                topLeftR2 + ( hdist * i ) );

            p2.place_text(
                math_mode( VSIZE_CORRECTION + p_substName ),
                topLeftR1 + ( hdist * i )
                    + tikz_point{ CHAR_WIDTH / 2.0,
                                  -CHAR_HEIGHT + .05
                                      + ( topLeftR2.m_y - ( topLeftR1.m_y - CHAR_HEIGHT ) ) / 2 },
                OPT::INNER_SEP( ".25em" ) | OPT::ANCHOR_EAST );
        }
    }
    out.add_picture( p2 );
    picture p3{ };
    {
        tikz_point topLeftR1{ 0.0, 0.0 }, topLeftR2{ 0.0, -2.5 * CHAR_HEIGHT };
        tikz_point hdist{ 1 * CHAR_WIDTH, 0.0 };
        for( u64 i = 0; i < p_wcSubst.size( ); ++i ) {
            p3.place_line(
                topLeftR1 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, -CHAR_HEIGHT - .03 },
                topLeftR2 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, 0.04 },
                OPT::double_arrow( OPT::DRAW( COLOR_TEXT ) ) | OPT::DRAW( COLOR_FILL_WHITE ) );
            p3.place_text(
                math_mode( VSIZE_CORRECTION + p_substName ),
                topLeftR1 + ( hdist * i )
                    + tikz_point{ CHAR_WIDTH / 2.0,
                                  -CHAR_HEIGHT + .05
                                      + ( topLeftR2.m_y - ( topLeftR1.m_y - CHAR_HEIGHT ) ) / 2 },
                OPT::INNER_SEP( ".25em" ) | OPT::ANCHOR_EAST );
        }
        place_string( p3, wildcard_string( { 0, p_wcSubst.size( ) }, true ), topLeftR1 );
        place_string(
            p3, stylized_string{ p_wcSubst, "S", str_displ_t::SHOW_CHARACTERS }.color_invert( ),
            topLeftR2 );
    }
    out.add_picture( p3 );
    picture p4{ };
    {
        tikz_point topLeftR1{ 0.0, 0.0 }, topLeftR2{ 0.0, -2.5 * CHAR_HEIGHT };
        tikz_point hdist{ 1 * CHAR_WIDTH, 0.0 };
        for( u64 i = 0; i < p_wcSubst.size( ); ++i ) {
            p4.place_line(
                topLeftR1 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, -CHAR_HEIGHT - .03 },
                topLeftR2 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, 0.04 },
                OPT::double_arrow( OPT::DRAW( COLOR_TEXT ) ) | OPT::DRAW( COLOR_FILL_WHITE ) );
        }
        p4.place_text(
            math_mode( VSIZE_CORRECTION + p_substName ),
            topLeftR1 + ( hdist * -.5 )
                + tikz_point{ CHAR_WIDTH / 2.0,
                              -CHAR_HEIGHT + .05
                                  + ( topLeftR2.m_y - ( topLeftR1.m_y - CHAR_HEIGHT ) ) / 2 },
            OPT::INNER_SEP( ".25em" ) | OPT::ANCHOR_EAST );

        place_string( p4, wildcard_string( { 0, p_wcSubst.size( ) }, true ), topLeftR1 );
        place_string(
            p4, stylized_string{ p_wcSubst, "S", str_displ_t::SHOW_CHARACTERS }.color_invert( ),
            topLeftR2 );
    }
    out.add_picture( p4 );
    picture p5{ };
    {
        tikz_point topLeftR1{ 0.0, 0.0 }, topLeftR2{ 0.0, -2.5 * CHAR_HEIGHT };
        tikz_point hdist{ 1 * CHAR_WIDTH, 0.0 };

        const auto& str = T_NAME;
        for( u64 i = 0, j = 0; i < str.length( ); ++i ) {
            if( !str.has_wildcard( i ) ) { continue; }

            u64 js = j, is = i;
            for( ; i < str.length( ) && str.has_wildcard( i ); ++i, ++j ) {
                p5.place_line(
                    topLeftR1 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, -CHAR_HEIGHT - .03 },
                    topLeftR2 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, 0.04 },
                    OPT::double_arrow( OPT::DRAW( COLOR_TEXT ) ) | OPT::DRAW( COLOR_FILL_WHITE ) );
                p5.place_text(
                    math_mode( VSIZE_CORRECTION + p_substName ),
                    topLeftR1 + ( hdist * i )
                        + tikz_point{ CHAR_WIDTH / 2.0,
                                      -CHAR_HEIGHT + .05
                                          + ( topLeftR2.m_y - ( topLeftR1.m_y - CHAR_HEIGHT ) )
                                                / 2 },
                    OPT::INNER_SEP( ".25em" ) | OPT::ANCHOR_EAST );
            }
            place_string( p5,
                          stylized_string{ p_wcSubst, "S", str_displ_t::SHOW_CHARACTERS }
                              .slice( { js, j } )
                              .color_invert( ),
                          topLeftR2 + ( hdist * is ) );
        }
        place_string( p5, str, topLeftR1 );
    }
    out.add_picture( p5 );
    picture p6{ };
    {
        auto Tr         = substitute_wildcards( T, p_wcSubst );
        auto occspt_all = compute_occs_with_mism( P, Tr, P.size( ) );
        auto TNr        = T_NAME.fill_wildcards( p_wcSubst );
        TNr.m_name      = p_substName + "(" + T_NAME.m_name + ")";
        wildcards_picture( "g" + p_name, P_NAME, TNr, occspt_all );

        tikz_point topLeftR1{ 0.0, 0.0 }, topLeftR2{ 0.0, -2.5 * CHAR_HEIGHT };
        tikz_point hdist{ 1 * CHAR_WIDTH, 0.0 };

        const auto& str = T_NAME;
        for( u64 i = 0, j = 0; i < str.length( ); ++i ) {
            if( !str.has_wildcard( i ) ) { continue; }
            for( ; i < str.length( ) && str.has_wildcard( i ); ++i, ++j ) {
                p6.place_line(
                    topLeftR1 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, -CHAR_HEIGHT - .03 },
                    topLeftR2 + ( hdist * i ) + tikz_point{ CHAR_WIDTH / 2.0, 0.04 },
                    OPT::double_arrow( OPT::DRAW( COLOR_TEXT ) ) | OPT::DRAW( COLOR_FILL_WHITE ) );
                p6.place_text(
                    math_mode( VSIZE_CORRECTION + p_substName ),
                    topLeftR1 + ( hdist * i )
                        + tikz_point{ CHAR_WIDTH / 2.0,
                                      -CHAR_HEIGHT + .05
                                          + ( topLeftR2.m_y - ( topLeftR1.m_y - CHAR_HEIGHT ) )
                                                / 2 },
                    OPT::INNER_SEP( ".25em" ) | OPT::ANCHOR_EAST );
            }
        }
        place_string( p6, str, topLeftR1 );
        place_string( p6, TNr, topLeftR2 );

        p6.place_text( math_mode( VSIZE_CORRECTION + TNr.m_name ),
                       topLeftR2 + tikz_point{ -1 * CHAR_WIDTH, -CHAR_HEIGHT / 2 } );
        p6.place_text( math_mode( VSIZE_CORRECTION + str.m_name ),
                       topLeftR1 + tikz_point{ -1 * CHAR_WIDTH, -CHAR_HEIGHT / 2 } );
    }
    out.add_picture( p6 );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void trie_picture( const std::string& p_name = "g03.tex" ) {
    trie                    T;
    std::deque<std::string> str{ "aba", "ac", "aca", "cba", "abc", "cb" };
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

void multi_trie_picture( const std::string& p_name = "g04.tex" ) {
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
    wildcards_picture( "g01b.tex", P_NAME, T_NAME, OCCS_P_T_ALL );
    wc_subst_picture( );
    wc_subst_picture( "g02b.tex", SUBST_BAD );
    trie_picture( );
    // multi_trie_picture( );
}
