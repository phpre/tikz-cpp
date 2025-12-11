#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font_talk";
std::string COLOR_FILENAME  = "color_talk";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

void add_pillar_meta_picture( document& p_doc, const kv_store& p_metaOptions = { } ) {
}

void add_pillar_implementation_picture( document& p_doc, const std::string& p_impName,
                                        const kv_store& p_impOptions = { } ) {
}

void add_pillar_combined_picture( document& p_doc, const std::string& p_impName,
                                  const kv_store& p_metaAptions = { },
                                  const kv_store& p_impOptions  = { } ) {
}

void add_standard_trick_pictures( document& p_doc, u64 p_n = 12, u64 p_m = 3, u64 p_k = 1,
                                  u64 p_l = 4 ) {
    auto tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };
    auto pn = stylized_string{ "P", fragmentco{ 0, p_m }, str_displ_t::NAME };

    picture         p1{ };
    u64             fa1s = 3;
    breakpoint_repn fake_al1{
        breakpoint{ 0, fa1s },
        breakpoint{ p_m, fa1s + p_m, 0, 'a' },
        breakpoint{ p_m, fa1s + p_m + 1 },
    };
    p1.place_node( tikz_point{ ( 1 + p_n ) * CHAR_WIDTH, -3.5 * CHAR_HEIGHT } );
    p1.place_node( tikz_point{ ( -1 ) * CHAR_WIDTH, 0.0 } );
    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( i == fa1s ) {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                             COLOR_C2.deemphasize_weak( ).deemphasize( ),
                             COLOR_C2.to_bg( ).deemphasize( ) );
        }
    }
    place_string( p1, pn, tikz_point{ fa1s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    p_doc.add_picture( p1 );

    place_highlighted_occurrence( p1, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                  tikz_point{ 0.0, 0.0 }, { fake_al1 }, 0 );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p1, pn, tikz_point{ fa1s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );

    p_doc.add_picture( p1 );

    picture         p2{ };
    u64             fa2s = 3;
    breakpoint_repn fake_al2{
        breakpoint{ 0, fa2s },
        breakpoint{ p_m - 2, fa2s + p_m - 2, 'a', 0 },
        breakpoint{ p_m, fa2s + p_m - 1 },
    };
    p2.place_node( tikz_point{ ( 1 + p_n ) * CHAR_WIDTH, -3.5 * CHAR_HEIGHT } );
    p2.place_node( tikz_point{ ( -1 ) * CHAR_WIDTH, 0.0 } );

    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( i == fa2s ) {
            p2.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            p2.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                             COLOR_C2.deemphasize_weak( ).deemphasize( ),
                             COLOR_C2.to_bg( ).deemphasize( ) );
        }
    }

    place_highlighted_occurrence( p2, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                  tikz_point{ 0.0, 0.0 }, { fake_al2 }, 0 );
    place_string( p2, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p2, pn, tikz_point{ fa2s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
    p_doc.add_picture( p2 );

    u64             fa3s = p_n - p_m + 1;
    breakpoint_repn fake_al3{
        breakpoint{ 0, fa3s },
        breakpoint{ p_m - 1, fa3s + p_m - 1, 'a', 0 },
        breakpoint{ p_m, fa3s + p_m - 1 },
    };

    p1.place_node( tikz_point{ ( 1 + p_n ) * CHAR_WIDTH, -3.5 * CHAR_HEIGHT } );
    p1.place_node( tikz_point{ ( -1 ) * CHAR_WIDTH, 0.0 } );

    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( i == fa3s ) {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                             COLOR_C2.deemphasize_weak( ).deemphasize( ),
                             COLOR_C2.to_bg( ).deemphasize( ) );
        }
    }

    place_highlighted_occurrence( p1, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                  tikz_point{ 0.0, 0.0 }, { fake_al3 }, 0 );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p1, pn, tikz_point{ fa3s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
    p_doc.add_picture( p1 );

    // split into substrings
    for( u64 j = 0; j < p_n - p_m + p_k + 1; j += p_l ) {
        picture     p4{ };
        std::string name = "T\\fragmentco{";
        if( !j ) {
            name += "0";
        } else if( j / p_l == 1 ) {
            name += "\\ell";
        } else {
            name += std::format( "{}\\ell", j / p_l );
        }
        name += "}{";
        if( !j ) {
            name += " k + p + \\ell";
        } else if( j + p_l >= p_n - p_m + p_k + 1 ) {
            name += "t";
        } else {
            name += std::format( " k + p + {}\\ell", 1 + ( j / p_l ) );
        }
        name += "}";

        auto tn = stylized_string{ name, fragmentco{ j, std::min( p_n, j + p_m + p_l + p_k - 1 ) },
                                   str_displ_t::NAME };

        for( u64 i = j; i < std::min( p_n - p_m + p_k + 1, j + p_l ); ++i ) {
            if( i == fa3s ) {
                p4.place_hatena( tikz_point{ ( i - j + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
            } else {
                p4.place_hatena( tikz_point{ ( i - j + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                 COLOR_C2.deemphasize_weak( ).deemphasize( ),
                                 COLOR_C2.to_bg( ).deemphasize( ) );
            }
        }

        if( j <= fa3s and fa3s < j + p_l ) {
            place_highlighted_occurrence( p4, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                          tikz_point{ 0.0, 0.0 }, { fake_al3 }, 0 );
            place_string( p4, pn, tikz_point{ ( fa3s - j ) * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
        }
        if( j <= fa1s and fa1s < j + p_l ) {
            place_highlighted_occurrence( p4, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                          tikz_point{ 0.0, 0.0 }, { fake_al1 }, 0 );
            place_string( p4, pn, tikz_point{ ( fa1s - j ) * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );
        }

        place_string( p4, tn, tikz_point{ 0.0, 0.0 } );

        p_doc.add_picture( p4 );
    }
}

void add_filter_verify_pictures( document& p_doc, u64 p_n = 12, u64 p_m = 3, u64 p_k = 1,
                                 u64 p_srand = 0, u64 p_batch = 3 ) {
    srand( p_srand );
    auto tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };

    WITH_PICTURE( p1, { }, p_doc ) {
        p1.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
        place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
        for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
            p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        }
    }

    std::set<u64> left{ };
    WITH_PICTURE( p2, { }, p_doc ) {
        p2.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
        place_string( p2, tn, tikz_point{ 0.0, 0.0 } );
        for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
            if( rand( ) % 13 < 6 ) {
                p2.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
            } else {
                left.insert( i );
                p2.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
            }
        }
    }
    std::set<u64> good{ };
    WITH_PICTURE( p3, { }, p_doc ) {
        p3.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
        place_string( p3, tn, tikz_point{ 0.0, 0.0 } );
        for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
            if( left.count( i ) ) {
                if( rand( ) % 7 < 3 ) {
                    p3.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                } else {
                    good.insert( i );
                    p3.place_maru( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                }
            } else {
                p3.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                COLOR_C3.deemphasize_weak( ).deemphasize( ),
                                COLOR_C3.deemphasize( ).deemphasize( ) );
            }
        }
    }

    for( auto n : left ) {
        WITH_PICTURE( p4, { }, p_doc ) {
            p4.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );

            for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
                if( left.count( i ) ) {
                    if( i < n && good.count( i ) ) {
                        p4.place_maru( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                    } else if( i < n ) {
                        p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                    } else if( i == n ) {
                        p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                         COLOR_C2.deemphasize_weak( ), COLOR_C2.to_bg( ),
                                         OPT::FILL( COLOR_C5.to_bg( ) ) | OPT::RECTANGLE
                                             | OPT::ROUNDED_CORNERS );
                    } else {
                        p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                         COLOR_C2.deemphasize_weak( ).deemphasize( ),
                                         COLOR_C2.to_bg( ).deemphasize( ) );
                    }
                } else {
                    p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                    COLOR_C3.deemphasize_weak( ).deemphasize( ),
                                    COLOR_C3.deemphasize( ).deemphasize( ) );
                }
            }
            place_string( p4, tn, tikz_point{ 0.0, 0.0 } );
        }
    }

    while( left.size( ) % p_batch ) { left.insert( p_n + left.size( ) ); }

    std::set<u64> batch{ };
    for( auto n : left ) {
        batch.insert( n );
        if( batch.size( ) < p_batch ) { continue; }

        WITH_PICTURE( p4, { }, p_doc ) {
            p4.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );

            for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
                if( left.count( i ) ) {
                    if( i < *batch.begin( ) && good.count( i ) ) {
                        p4.place_maru( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                    } else if( i < *batch.begin( ) ) {
                        p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                    } else if( batch.count( i ) ) {
                        p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                         COLOR_C2.deemphasize_weak( ), COLOR_C2.to_bg( ),
                                         OPT::FILL( COLOR_C5.to_bg( ) ) | OPT::RECTANGLE
                                             | OPT::ROUNDED_CORNERS );
                    } else {
                        p4.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                         COLOR_C2.deemphasize_weak( ).deemphasize( ),
                                         COLOR_C2.to_bg( ).deemphasize( ) );
                    }
                } else {
                    p4.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 },
                                    COLOR_C3.deemphasize_weak( ).deemphasize( ),
                                    COLOR_C3.deemphasize( ).deemphasize( ) );
                }
            }
            place_string( p4, tn, tikz_point{ 0.0, 0.0 } );
        }
        batch.clear( );
    }
}

void add_pt_pictures( document& p_doc, u64 p_n = 12, u64 p_m = 3 ) {
    auto tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };
    auto pn = stylized_string{ "P", fragmentco{ 0, p_m }, str_displ_t::NAME };

    WITH_PICTURE( p1, { }, p_doc ) {
        place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
        place_string( p1, pn, tikz_point{ ( p_n - p_m ) / 2.0 * CHAR_WIDTH, -1.5 * CHAR_HEIGHT } );
    }
    WITH_PICTURE( p2, { }, p_doc ) {
        place_string( p2, tn.color_invert( ), tikz_point{ 0.0, 0.0 } );
        place_string( p2, pn.color_invert( ),
                      tikz_point{ ( p_n - p_m ) / 2.0 * CHAR_WIDTH, -1.5 * CHAR_HEIGHT } );
    }
}

void add_structural_insight_1_pictures( document&   p_doc,
                                        occ_style_t p_style = occ_style_t::OCC_FULL, u64 p_n = 16,
                                        u64 p_m = 8, u64 p_k = 3 ) {
    // T aaaabbbb
    // P   aabb

    std::string T{ }, P{ };
    for( u64 i = 0; i < p_n / 2; ++i ) { T += "a"; }
    for( u64 i = 0; i < p_m / 2; ++i ) { P += "a"; }

    for( u64 i = 0; i < p_n / 2; ++i ) { T += "b"; }
    for( u64 i = 0; i < p_m / 2; ++i ) { P += "b"; }

    auto occs = compute_occs_with_mism( P, T, p_k );

    auto tn = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    for( u64 i = 0; i < p_n / 2; ++i ) {
        tn.annotation_at_pos( p_n / 2 + i ).m_displayStyle
            |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
    }
    for( u64 i = 0; i < p_m / 2; ++i ) {
        pn.annotation_at_pos( p_m / 2 + i ).m_displayStyle
            |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
    }

    // reorder occs
    auto occsre = std::deque<breakpoint_repn>{ };
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT >= p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }
    std::reverse( occs.begin( ), occs.end( ) );
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT < p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }

    add_occurrences_pictures( p_doc, pn, tn, occsre, p_style );
}

void add_structural_insight_2_pictures( document&   p_doc,
                                        occ_style_t p_style = occ_style_t::OCC_FULL, u64 p_n = 16,
                                        u64 p_m = 8, u64 p_k = 4, u64 p_srand = 0 ) {
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

    auto occs = compute_occs_with_mism( P, T, p_k );

    auto tn = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    for( u64 i = 0; i < p_n; ++i ) {
        if( T[ i ] == 'b' ) {
            tn.annotation_at_pos( i ).m_displayStyle |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
        }
    }
    for( u64 i = 0; i < p_m; ++i ) {
        if( P[ i ] == 'b' ) {
            pn.annotation_at_pos( i ).m_displayStyle |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
        }
    }
    add_occurrences_pictures( p_doc, pn, tn, occs, p_style );
}

void add_structural_insight_3_pictures( document&   p_doc,
                                        occ_style_t p_style = occ_style_t::OCC_FULL, u64 p_n = 24,
                                        u64 p_m = 12, u64 p_k = 2 ) {
    // T aaaaaaaaaaaa baabaabaabaa
    // P       aaaaaa baabaa

    std::string T{ }, P{ };
    for( u64 i = 0; i < p_n / 6; ++i ) { T += "aaa"; }
    for( u64 i = 0; i < p_m / 6; ++i ) { P += "aaa"; }

    for( u64 i = 0; i < p_n / 6; ++i ) { T += "baa"; }
    for( u64 i = 0; i < p_m / 6; ++i ) { P += "baa"; }

    auto occs = compute_occs_with_edits( P, T, p_k );

    auto tn = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
    auto pn = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    for( u64 i = 0; i < p_n / 2; i += 3 ) {
        tn.annotation_at_pos( p_n / 2 + i ).m_displayStyle
            |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
    }
    for( u64 i = 0; i < p_m / 2; i += 3 ) {
        pn.annotation_at_pos( p_m / 2 + i ).m_displayStyle
            |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
    }

    // reorder occs
    auto occsre = std::deque<breakpoint_repn>{ };
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT >= p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }
    std::reverse( occs.begin( ), occs.end( ) );
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT < p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }

    add_occurrences_pictures( p_doc, pn, tn, occsre, p_style );
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures; one file per frame
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( p_f_wapm_kt, {
    constexpr std::string T      = "AAAcdaAAAaacAAaddAAAAA";
    constexpr std::string P      = "AbAAbcdAAAdAAbddAAaAAA";
    const breakpoint_repn BP_P_T = compute_breakpoints( P, T );

    constexpr std::string T2       = "DEFGHaJKLMcOPQAB";
    constexpr std::string P2       = "DEFGHIJKLMNOPQAB";
    const breakpoint_repn BP_P2_T2 = compute_breakpoints( P2, T2 );

    const stylized_string P_NAME2{ std::string{ "P" }, fragmentco{ 0, P.size( ) } };
    const stylized_string T_NAME2{ std::string{ "T" }, fragmentco{ 0, T.size( ) } };
    const stylized_string P_NAME{ P, std::string{ "P" }, str_displ_t::FRAGMENT };
    const stylized_string S_NAME{ std::string{ "S" }, fragmentco{ 0, 0 } };
    const stylized_string T2_NAME{ T2, std::string{ "T" }, str_displ_t::FRAGMENT };
    const stylized_string P2_NAME{ P2, std::string{ "P" }, str_displ_t::FRAGMENT };

    bool cf    = CROSS_FILL;
    CROSS_FILL = false;
    add_filter_verify_pictures( doc, 12, 3, 1, 0, 3 );

    add_alignment_picture( doc, BP_P2_T2, T2_NAME, P2_NAME, AT_COMPRESS );
    add_alignment_picture(
        doc, breakpoint_slice( BP_P_T, { 0, BP_P2_T2.back( ).m_posP } ),
        T_NAME2.slice( align_fragment( BP_P_T, { 0, BP_P2_T2.back( ).m_posP } ) ),
        P_NAME2.slice( { 0, BP_P2_T2.back( ).m_posP } ) );

    add_slices_pictures( doc, P_NAME, S_NAME, fragmentco{ 0, 11 }, 1, BP_P_T, P, T );

    add_alignment_graph_explanation_pictures( doc );

    CROSS_FILL = cf;
} )
FILE_SIMPLE( p_f_apm_standard_trick, { add_standard_trick_pictures( doc ); } )
FILE_SIMPLE( p_f_qapm_concl, { add_pt_pictures( doc ); } )
FILE_SIMPLE( p_f_qapm_intro, { add_standard_trick_pictures( doc ); } )
FILE_SIMPLE( p_f_def_apm_ed, {
    bool cf    = CROSS_FILL;
    CROSS_FILL = true;
    add_alignment_picture( doc,
                           "saarbr\xfc"
                           "cken",
                           "saarbruecken" );
    add_alignment_picture( doc,
                           "saarbr\xfc"
                           "cken",
                           "sarrebruck" );
    CROSS_FILL = cf;
} )
FILE_SIMPLE( p_f_def_ed, {
    bool cf    = CROSS_FILL;
    CROSS_FILL = true;
    cost_table w{
        { { '0', 'O' }, 1 },
        { { '1', 'I' }, 1 },
        { { 'C', 'O' }, 1 },
    };
    std::string alph = "0PIN1CIVOPINIONPICNIC";
    for( auto c : alph ) {
        for( auto d : alph ) {
            if( !w.count( { c, d } ) ) { w[ { c, d } ] = 2; }
        }
        if( !w.count( { c, 0 } ) ) { w[ { c, 0 } ] = 1; }
        if( !w.count( { 0, c } ) ) { w[ { 0, c } ] = 10; }
    }

    add_alignment_picture( doc, "0PIN1CIV", "OPINION", w,
                           AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS );

    breakpoint_repn fake_al{
        breakpoint{ 0, 0 },
        breakpoint{ 0, 0, 1, '0', 0 },
        breakpoint{ 3, 2, 10, 0, 'C' },
        breakpoint{ 4, 4, 1, '1', 'I' },
        breakpoint{ 6, 6, 1, 'I', 0 },
        breakpoint{ 7, 6, 1, 'V', 0 },
        breakpoint{ 8, 6 },
    };

    add_alignment_picture( doc, "0PIN1CIV", "PICNIC", fake_al,
                           AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS );
    CROSS_FILL = cf;
} )
FILE_SIMPLE( p_f_def_wed, {
    cost_table w{
        { { '0', 'O' }, 1 },
        { { '1', 'I' }, 1 },
        { { 'C', 'O' }, 1 },
    };
    std::string alph = "0PIN1CIVOPINIONPICNIC";
    for( auto c : alph ) {
        for( auto d : alph ) {
            if( !w.count( { c, d } ) ) { w[ { c, d } ] = 2; }
        }
        if( !w.count( { c, 0 } ) ) { w[ { c, 0 } ] = 1; }
        if( !w.count( { 0, c } ) ) { w[ { 0, c } ] = 10; }
    }

    breakpoint_repn fake_al{
        breakpoint{ 0, 0 },
        breakpoint{ 0, 0, 1, '0', 0 },
        breakpoint{ 3, 2, 10, 0, 'C' },
        breakpoint{ 4, 4, 1, '1', 'I' },
        breakpoint{ 6, 6, 1, 'I', 0 },
        breakpoint{ 7, 6, 1, 'V', 0 },
        breakpoint{ 8, 6 },
    };
    add_alignment_picture( doc, "0PIN1CIV", "OPINION", w,
                           AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS | AT_SHOW_EDIT_COST );
    add_alignment_picture( doc, "0PIN1CIV", "PICNIC", fake_al,
                           AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS | AT_SHOW_EDIT_COST );
    add_alignment_picture( doc, "0PIN1CIV", "PICNIC", w,
                           AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS | AT_SHOW_EDIT_COST );
} )
FILE_SIMPLE( p_f_pmwe_str_examples_mr, {
    bool cf    = CROSS_FILL;
    CROSS_FILL = true;
    add_structural_insight_3_pictures( doc, occ_style_t::NO_ANNOTATION );
    CROSS_FILL = cf;
} )
FILE_SIMPLE( p_f_pmwm_str_examples_mr, {
    bool cf    = CROSS_FILL;
    CROSS_FILL = true;
    add_structural_insight_1_pictures( doc, occ_style_t::NO_ANNOTATION );
    add_structural_insight_2_pictures( doc, occ_style_t::NO_ANNOTATION );
    CROSS_FILL = cf;
} )
FILE_SIMPLE( p_f_wed_intro_example, {
    add_string_picture( doc, "OPINION" );
    add_string_picture( doc, "0PIN1CIV" );
    add_string_picture( doc, "PICNIC" );
} )
FILE_SIMPLE( p_f_apm_filter_verify, {
    bool cf    = CROSS_FILL;
    CROSS_FILL = false;
    add_filter_verify_pictures( doc, 12, 3, 1, 0, 3 );
    CROSS_FILL = cf;
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
