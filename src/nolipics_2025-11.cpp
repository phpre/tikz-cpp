#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      globals
//
// ---------------------------------------------------------------------------------------------

std::string T = "bb" + WILDCARD + WILDCARD + WILDCARD + "a" + WILDCARD + "ab" + WILDCARD + WILDCARD
                + "a" + WILDCARD + "a";
std::string P = "bbaaa";

std::string SUBST_GOOD = "aabbaaa";
std::string SUBST_BAD  = "abcabca";

auto T_NAME = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS }
                  .add_wildcards( T, chr_displ_t::SHOW_ID_IF_WILDCARD );
auto P_NAME = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

auto OCCS_P_T     = compute_occs( P, T, WILDCARD );
auto OCCS_P_T_ALL = compute_occs_with_mism( P, T, P.size( ), WILDCARD );

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

void add_wc_subst_pictures( document& p_doc, const std::string p_wcSubst = SUBST_GOOD,
                            const std::string      p_substName = "\\varphi",
                            const stylized_string& p_t         = T_NAME,
                            const stylized_string& p_p         = P_NAME ) {
    WITH_PICTURE( p1, { }, p_doc ) {
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
    WITH_PICTURE( p2, { }, p_doc ) {
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
    WITH_PICTURE( p3, { }, p_doc ) {
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
    WITH_PICTURE( p4, { }, p_doc ) {
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
    WITH_PICTURE( p5, { }, p_doc ) {
        tikz_point topLeftR1{ 0.0, 0.0 }, topLeftR2{ 0.0, -2.5 * CHAR_HEIGHT };
        tikz_point hdist{ 1 * CHAR_WIDTH, 0.0 };

        const auto& str = p_t;
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
    WITH_PICTURE( p6, { }, p_doc ) {
        auto Tr         = substitute_wildcards( T, p_wcSubst );
        auto occspt_all = compute_occs_with_mism( P, Tr, P.size( ) );
        auto TNr        = p_t.fill_wildcards( p_wcSubst );
        TNr.m_name      = p_substName + "(" + p_t.m_name + ")";

        tikz_point topLeftR1{ 0.0, 0.0 }, topLeftR2{ 0.0, -2.5 * CHAR_HEIGHT };
        tikz_point hdist{ 1 * CHAR_WIDTH, 0.0 };

        const auto& str = p_t;
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

        add_occurrences_pictures( p_doc, p_p, TNr, occspt_all );
    }
}

void add_multi_trie_pictures( document& p_doc, const std::string& p_t = T,
                              const std::string& p_p = P, const std::string& p_alphabet = "ab",
                              const std::string& p_wildcard = WILDCARD ) {
    stylized_string pN{ p_p, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    stylized_string tN
        = stylized_string{ p_t, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS }
              .add_wildcards( p_t, chr_displ_t::SHOW_ID_IF_WILDCARD, 0, p_wildcard );

    // compute occs w/ wildcards of p in t.
    auto occs  = compute_occs( p_p, p_t, p_wildcard );
    u64  numwc = 0;

    std::deque<std::pair<u64, std::string>> constraints{ };
    std::deque<stylized_string>             cstr{ };

    WITH_PICTURE( p1, { }, p_doc ) {
        p1.place_text( math_mode( VSIZE_CORRECTION + tN.m_name ),
                       tikz_point{ -1 * CHAR_WIDTH, -CHAR_HEIGHT / 2 } );

        for( u64 i = 0; i < p_t.size( ); ++i ) {
            if( tN.has_wildcard( i ) ) { numwc++; }
        }

        for( u64 y = occs.size( ); y-- > 0; ) {
            p1.place_text( math_mode( VSIZE_CORRECTION + pN.m_name ),
                           tikz_point{ -1 * CHAR_WIDTH, -1.25 - ( 1.5 * y - .5 ) * CHAR_HEIGHT } );
            place_alignment( p1, pN, tikz_point{ 0.0, -.75 - 1.5 * y * CHAR_HEIGHT }, tN,
                             tikz_point{ 0.0, 0.0 }, occs[ y ], AT_OCCS_DEFAULT );

            std::string constr{ }, cwc{ };
            u64         prefix = p_t.size( ) + 1;

            for( const auto& bp : occs[ y ] ) {
                if( !bp.m_charP || !bp.m_charT ) { continue; }
                if( !tN.has_wildcard( bp.m_posT ) ) { continue; }

                prefix = std::min( prefix, tN.annotation_at_pos( bp.m_posT ).m_wildcardId );
                constr += bp.m_charP;
                cwc += bp.m_charP;
            }
            // pad cwc with wildcards
            std::string wcp{ };
            for( u64 i = 0; i < prefix; ++i ) { wcp += p_wildcard; }
            cwc = wcp + cwc;

            while( cwc.size( ) < numwc ) { cwc += p_wildcard; }

            auto c = stylized_string{ cwc, "C_{" + std::to_string( y ) + "}",
                                      str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS }
                         .add_wildcards( cwc, chr_displ_t::DEFAULT, 0, p_wildcard );

            p1.place_text( math_mode( VSIZE_CORRECTION + c.m_name ),
                           tikz_point{ ( 3 + p_t.length( ) ) * CHAR_WIDTH - 1 * CHAR_WIDTH,
                                       -1.25 - ( 1.5 * y - .5 ) * CHAR_HEIGHT } );
            place_string(
                p1, c,
                tikz_point{ ( 3 + p_t.length( ) ) * CHAR_WIDTH, -.75 - 1.5 * y * CHAR_HEIGHT } );

            constraints.push_front( { prefix, constr } );
            cstr.push_front( c );
        }
    }

    std::deque<trie> multi_trie{ numwc + 1, trie{} };

    double charDiv = 1.25 * DEFAULT_TRIE_CHAR_DIVERTION;
    double distX   = DEFAULT_TRIE_VERTEX_DIST_X;
    double distY   = 1.15 * DEFAULT_TRIE_VERTEX_DIST_Y;

    // push things into tries
    for( const auto& [ s, c ] : constraints ) {
        WITH_PICTURE( p2, { }, p_doc ) {
            multi_trie[ s ].insert( c );

            std::deque<placed_trie> ptr;
            u64                     height = 0;
            for( u64 i = 0; i <= numwc; ++i ) {
                auto tr = place_trie_wide( p2, multi_trie[ i ], p_alphabet,
                                           !i ? tikz_point{ 0.0, 0.0 }
                                              : ptr[ 0 ].vertex_position( i, height ),
                                           std::to_string( i ), distX, distY, charDiv );
                if( i == s ) {
                    place_diverted_trie_string_on_coordinates( p2, tr, c, p_alphabet, charDiv,
                                                               OPT::COLOR( COLOR_C3 ) );
                }

                height += multi_trie[ i ].m_vertices[ 0 ][ 0 ].m_size;
                ptr.push_back( tr );
            }
            if( !ptr.empty( ) ) {
                place_trie_depth_labels( p2, ptr[ 0 ], numwc,
                                         tikz_point{ 0, CHAR_HEIGHT * 3 / 4 } );
            }
        }
    }

    // add dummy edges
    WITH_PICTURE( p2, { }, p_doc ) {
        std::deque<placed_trie> ptr;
        u64                     height = 0;
        for( u64 i = 0; i <= numwc; ++i ) {
            auto tr = place_trie_wide( p2, multi_trie[ i ], p_alphabet,
                                       !i ? tikz_point{ 0.0, 0.0 }
                                          : ptr[ 0 ].vertex_position( i, height ),
                                       std::to_string( i ), distX, distY, charDiv );
            ptr.push_back( tr );
            height += multi_trie[ i ].m_vertices[ 0 ][ 0 ].m_size;
        }
        place_trie_depth_labels( p2, ptr[ 0 ], numwc, tikz_point{ 0, CHAR_HEIGHT * 3 / 4 } );
        p_doc.add_picture( p2 );

        std::map<tikz_point, std::set<char>> extra_edges{ };

        std::deque<std::deque<u64>> rems{ };
        for( u64 i = 0; i <= numwc; ++i ) {
            if( i ) {
                std::deque<u64> rem{ };
                for( u64 j = 0; j < p_alphabet.size( ); ++j ) {
                    auto c = p_alphabet[ j ];
                    if( !multi_trie[ i - 1 ].m_vertices[ 0 ][ 0 ].m_next.count( c ) ) {
                        rem.push_back( j );
                    }
                }
                rems.push_back( rem );

                if( !rem.empty( ) ) {
                    for( u64 j = rem.size( ); j-- > 0; ) {
                        extra_edges[ ptr[ i - 1 ][ TRIE_ROOT ].m_pos ].insert(
                            p_alphabet[ rem[ j ] ] );

                        place_diverted_trie_edge(
                            p2, ptr[ i - 1 ][ TRIE_ROOT ], ptr[ i ][ TRIE_ROOT ],
                            stylized_string{ std::string{ p_alphabet[ rem[ j ] ] }, EMPTY_STR,
                                             str_displ_t::SHOW_CHARACTERS },
                            j * distY, ( 2.25 + p_alphabet.size( ) - rem[ j ] ) * charDiv,
                            j ? OPT::COLOR( COLOR_C2.deemphasize( ) )
                              : OPT::COLOR( COLOR_C2.deemphasize_weak( ) ),
                            .75 );
                    }
                }
            }
        }
        p_doc.add_picture( p2 );

        // add dummy edges (2):
        // for each non-edge, divert to next trie that has a corresponding (dummy) edge
        auto opts  = OPT::COLOR( COLOR_C5.deemphasize( ) );
        auto opts2 = OPT::COLOR( COLOR_C5.deemphasize_weak( ) );
        for( const auto& [ s, cstr ] : constraints ) {
            // for each previous trie, try to insert c starting from depth s
            for( u64 i = 0; i < s; ++i ) {
                auto tg = s - i;
                if( multi_trie[ i ].m_vertices.size( ) <= tg ) {
                    // no string extends far enough
                    continue;
                }
                for( u64 j = 0; j < multi_trie[ i ].m_vertices[ tg ].size( ); ++j ) {
                    picture tmp{ p2 };
                    bool    broke     = false;
                    bool    brokehard = false;

                    trie_point posO{ TRIE_ROOT };
                    trie_point pos{ tg, j };
                    u64        i2 = 0;
                    for( ; i2 < cstr.size( ); ++i2 ) {
                        place_trie_vertex( tmp, ptr[ i ][ pos ], opts2 );

                        posO = ptr[ s ].next( posO, cstr[ i2 ] );
                        if( !ptr[ i ][ pos ].m_next.count( cstr[ i2 ] ) ) {
                            if( !ptr[ i ][ pos ].m_marked ) {
                                std::string c{ cstr[ i2 ] };
                                u64         cpos  = p_alphabet.find( c );
                                auto        label = stylized_string{ c };
                                place_diverted_trie_edge(
                                    p2, ptr[ i ][ pos ], ptr[ s ][ posO ], label, 0.0,
                                    ( 2.5 + p_alphabet.size( ) - cpos ) * charDiv, opts );

                                place_diverted_trie_edge(
                                    tmp, ptr[ i ][ pos ], ptr[ s ][ posO ], label, 0.0,
                                    ( 2.5 + p_alphabet.size( ) - cpos ) * charDiv, opts2 );

                                extra_edges[ ptr[ i ][ pos ].m_pos ].insert( p_alphabet[ cpos ] );
                            } else {
                                brokehard = true;
                            }
                            broke = true;
                            break;
                        }

                        auto        n = ptr[ i ].next( pos, cstr[ i2 ] );
                        std::string c{ cstr[ i2 ] };
                        u64         cpos  = p_alphabet.find( c );
                        auto        label = stylized_string{ c };
                        place_diverted_trie_edge( tmp, ptr[ i ][ pos ], ptr[ i ][ n ], label, 0.0,
                                                  ( 2.5 + p_alphabet.size( ) - cpos ) * charDiv,
                                                  opts2 );
                        pos = n;
                    }
                    if( !broke ) {
                        // TODO: mark this vertex
                        // place_trie_vertex( p2, ptr[ i ][ pos ], opts );
                        // place_trie_vertex( tmp, ptr[ i ][ pos ], opts2 );
                    }
                    if( !brokehard ) { p_doc.add_picture( tmp ); }
                }
            }
        }
        p_doc.add_picture( p2 );

        // finally, complete each internal vertex

        for( u64 i = 0; i < numwc; ++i ) {
            auto vtcs = ptr[ i ].in_order( );
            for( auto p = vtcs.begin( ); p != vtcs.end( ); ++p ) {
                for( auto c : p_alphabet ) {
                    if( ptr[ i ][ *p ].m_next.count( c ) ) { continue; }
                    if( extra_edges[ ptr[ i ][ *p ].m_pos ].count( c ) ) { continue; }
                    if( ptr[ i ][ *p ].m_marked ) { continue; }

                    // c is missing. Divert to dummy trie

                    u64  cpos  = p_alphabet.find( std::string{ c } );
                    auto label = stylized_string{ std::string{ c } };

                    place_diverted_trie_edge(
                        p2, ptr[ i ][ *p ], ptr[ p->first + 1 + i ][ TRIE_ROOT ], label,
                        cpos * distY, ( 2.75 + p_alphabet.size( ) - cpos ) * charDiv,
                        OPT::COLOR( COLOR_C1.deemphasize( ) ), .75 );
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, {
    add_occurrences_pictures( doc, P_NAME, T_NAME, OCCS_P_T );
    add_occurrences_pictures( doc, P_NAME, T_NAME, OCCS_P_T_ALL );
} )
FILE_SIMPLE( g02, {
    add_wc_subst_pictures( doc );
    add_wc_subst_pictures( doc, SUBST_BAD );
} )
FILE_SIMPLE( g03, {
    add_trie_construction_pictures( doc, EMPTY_STR, { "aba", "ac", "aca", "cba", "abc", "cb" } );
    add_trie_construction_pictures( doc, "abc", { "aba", "ac", "aca", "cba", "abc", "cb" } );
    add_trie_construction_pictures( doc, "abc",
                                    { "aaaa", "aabb", "aacc", "abaa", "acaa", "abba" } );
} )
FILE_SIMPLE( g04, { add_multi_trie_pictures( doc ); } )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
