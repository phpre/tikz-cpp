#include "tikz.h"
using namespace TIKZ;

std::string OUT_DIR      = "../figs/";
std::string TEX_DIR      = "../tex/";
std::string FONT_PATH    = TEX_DIR + "font_talk";
std::string COLOR_PATH   = TEX_DIR + "color_talk";
std::string MACRO_PATH   = TEX_DIR + "macros";
std::string PROGRAM_NAME = "";

void picture_filter_verify( const std::string& p_name = "g02.tex", u64 p_n = 12, u64 p_m = 3,
                            u64 p_k = 1, u64 p_srand = 0, u64 p_batch = 3 ) {
    srand( p_srand );

    auto     tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };
    document out{ };
    picture  p1{ };

    p1.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        p1.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
    }

    out.add_picture( p1 );
    picture p2{ };
    p2.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
    place_string( p2, tn, tikz_point{ 0.0, 0.0 } );
    std::set<u64> left{ };
    for( u64 i = 0; i < p_n - p_m + p_k + 1; ++i ) {
        if( rand( ) % 13 < 6 ) {
            p2.place_batsu( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        } else {
            left.insert( i );
            p2.place_hatena( tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
        }
    }
    out.add_picture( p2 );

    picture p3{ };
    p3.place_node( tikz_point{ 0.0, .9 * CHAR_HEIGHT } );
    place_string( p3, tn, tikz_point{ 0.0, 0.0 } );
    std::set<u64> good{ };
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
    out.add_picture( p3 );

    for( auto n : left ) {
        picture p4{ };
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
        out.add_picture( p4 );
    }

    while( left.size( ) % p_batch ) { left.insert( p_n + left.size( ) ); }

    std::set<u64> batch{ };
    for( auto n : left ) {
        batch.insert( n );
        if( batch.size( ) < p_batch ) { continue; }

        picture p4{ };
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
        out.add_picture( p4 );
        batch.clear( );
    }

    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_standard_trick( const std::string& p_name = "g01.tex", u64 p_n = 12, u64 p_m = 3,
                             u64 p_k = 1, u64 p_l = 4 ) {
    auto tn = stylized_string{ "T", fragmentco{ 0, p_n }, str_displ_t::NAME };
    auto pn = stylized_string{ "P", fragmentco{ 0, p_m }, str_displ_t::NAME };

    document        out{ };
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
    out.add_picture( p1 );

    place_highlighted_occurrence( p1, pn, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT }, tn,
                                  tikz_point{ 0.0, 0.0 }, { fake_al1 }, 0 );
    place_string( p1, tn, tikz_point{ 0.0, 0.0 } );
    place_string( p1, pn, tikz_point{ fa1s * CHAR_WIDTH, -2.5 * CHAR_HEIGHT } );

    out.add_picture( p1 );

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
    out.add_picture( p2 );

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
    out.add_picture( p1 );

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
            name += " k + |P| + \\ell";
        } else if( j + p_l >= p_n - p_m + p_k + 1 ) {
            name += "|T|";
        } else {
            name += std::format( " k + |P| + {}\\ell", 1 + ( j / p_l ) );
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

        out.add_picture( p4 );
    }

    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_structural_insight1( const std::string& p_name = "g11.tex",
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

    for( u64 i = 0; i < p_n / 2; ++i ) { tn.annotation_at_pos( p_n / 2 + i ).m_wasWildcard = true; }
    for( u64 i = 0; i < p_m / 2; ++i ) { pn.annotation_at_pos( p_m / 2 + i ).m_wasWildcard = true; }

    // reorder occs
    auto occsre = std::deque<breakpoint_repn>{ };
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT >= p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }
    std::reverse( occs.begin( ), occs.end( ) );
    for( const auto& o : occs ) {
        if( o[ 0 ].m_posT < p_n / 2 - p_m / 2 ) { occsre.push_back( o ); }
    }

    document out{ };
    add_occurrences_pictures( out, pn, tn, occsre, p_style );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

void picture_structural_insight2( const std::string& p_name = "g12.tex",
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
        if( T[ i ] == 'b' ) { tn.annotation_at_pos( i ).m_wasWildcard = true; }
    }
    for( u64 i = 0; i < p_m; ++i ) {
        if( P[ i ] == 'b' ) { pn.annotation_at_pos( i ).m_wasWildcard = true; }
    }

    document out{ };
    add_occurrences_pictures( out, pn, tn, occs, p_style );
    document::output( OUT_DIR, p_name, out.render( FONT_PATH, COLOR_PATH, MACRO_PATH ) );
}

int main( int p_argc, char* p_argv[] ) {
    PROGRAM_NAME = p_argv[ 0 ];
    if( p_argc > 1 ) { OUT_DIR = std::string{ p_argv[ 1 ] }; }
    if( p_argc > 2 ) {
        TEX_DIR    = std::string{ p_argv[ 2 ] };
        FONT_PATH  = TEX_DIR + "font_talk";
        COLOR_PATH = TEX_DIR + "color_talk";
        MACRO_PATH = TEX_DIR + "macros";
    }

    CROSS_FILL = true;
    picture_structural_insight1( );
    picture_structural_insight1( "g11b.tex", occ_style_t::NO_ANNOTATION );
    picture_structural_insight2( );
    picture_structural_insight2( "g12b.tex", occ_style_t::NO_ANNOTATION );

    CROSS_FILL = false;
    picture_standard_trick( );
    picture_filter_verify( );
}
