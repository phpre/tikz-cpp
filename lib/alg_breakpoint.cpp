#include "alg_breakpoint.h"

namespace ALG {

    bool match( char p_P, char p_T, char p_wc ) {
        if( !p_P && !p_T ) {
            return true;
        } else if( !p_P || !p_T ) {
            return false;
        }

        if( !p_wc ) {
            return p_P == p_T;
        } else {
            return p_P == p_wc || p_T == p_wc || p_P == p_T;
        }
    }

    u64 get_cost( const cost_table& p_w, char p_p, char p_t, char p_wc ) {
        if( match( p_p, p_t, p_wc ) ) { return 0; }
        if( p_w.count( { p_p, p_t } ) ) { return p_w.at( { p_p, p_t } ); }
        return 1;
    }

    u64 lce( const std::string& p_s1, const std::string& p_s2, u64 p_i1, u64 p_i2, char p_wc ) {
        u64 lce = 0;
        while( p_i1 + lce < p_s1.length( ) && p_i2 + lce < p_s2.length( )
               && match( p_s1[ p_i1 + lce ], p_s2[ p_i2 + lce ], p_wc ) ) {
            ++lce;
        }
        return lce;
    }

    std::deque<breakpoint_repn> compute_occs_with_edits_lv( const std::string& p_P,
                                                            const std::string& p_T, u64 p_threshold,
                                                            const std::string& p_wildcard,
                                                            bool               p_wcInOutput,
                                                            ed_dp_cb_t         p_progressCB ) {
        std::string Pr{ p_P.rbegin( ), p_P.rend( ) };
        std::string Tr{ p_T.rbegin( ), p_T.rend( ) };

        u64  m  = p_P.size( );
        auto n  = p_T.size( );
        u64  wc = ( p_wildcard == EMPTY_STR || !p_wildcard.length( ) ) ? 0 : p_wildcard[ 0 ];

        // diagonal, errors
        u64  shift = m;
        u64  maxd  = n + m;
        auto dp    = std::deque<std::deque<u64>>{ maxd + 1, std::deque<u64>( p_threshold + 1, 0 ) };

        for( u64 d = 0; d <= n; ++d ) {
            dp[ d + shift ][ 0 ] = lce( Pr, Tr, 0, d, wc );
            if( p_progressCB != nullptr ) { p_progressCB( d + shift, 0, dp ); }
        }
        if( p_progressCB != nullptr ) { p_progressCB( m + n + 1, 0, dp ); }

        for( u64 l = 1; l <= p_threshold; ++l ) {
            for( s64 d = -l; d <= s64( n ); ++d ) {
                if( dp[ d + shift ][ l - 1 ] == m ) {
                    dp[ d + shift ][ l ] = m;
                    continue;
                }

                dp[ d + shift ][ l ] = dp[ d + shift ][ l - 1 ] + 1; // subst
                if( d + shift ) {
                    dp[ d + shift ][ l ]
                        = std::max( dp[ d + shift ][ l ], dp[ d + shift - 1 ][ l - 1 ] ); // ins
                }
                if( d + shift + 1 <= maxd ) {
                    dp[ d + shift ][ l ]
                        = std::max( dp[ d + shift ][ l ], dp[ d + shift + 1 ][ l - 1 ] + 1 ); // del
                }
                if( dp[ d + shift ][ l ] > m || d + dp[ d + shift ][ l ] > n ) {
                    dp[ d + shift ][ l ] = m;
                } else {
                    dp[ d + shift ][ l ]
                        += lce( Tr, Pr, d + dp[ d + shift ][ l ], dp[ d + shift ][ l ], wc );
                }
                if( p_progressCB != nullptr ) { p_progressCB( d + shift, l, dp ); }
            }
            if( p_progressCB != nullptr ) { p_progressCB( m + n + 1, l, dp ); }
        }

        // TODO: compute actual alignments of occ's
        (void) p_wcInOutput;
        return { };
    }

    breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T,
                                         const cost_table& p_w, const std::string& p_wildcard,
                                         bool p_wcInOutput, ed_dp_cb_t p_progressCB ) {
        auto m = p_P.size( );
        auto n = p_T.size( );

        char wc = ( p_wildcard == EMPTY_STR || !p_wildcard.length( ) ) ? 0 : p_wildcard[ 0 ];

        auto nw_dp = std::deque<std::deque<u64>>{ m + 1, std::deque<u64>( n + 1, 0 ) };
        for( u64 j = 1; j <= n; ++j ) {
            nw_dp[ 0 ][ j ] = nw_dp[ 0 ][ j - 1 ] + get_cost( p_w, 0, p_T[ j - 1 ], wc );
            if( p_progressCB != nullptr ) { p_progressCB( 0, j, nw_dp ); }
        }
        for( u64 i = 1; i <= m; ++i ) {
            nw_dp[ i ][ 0 ] = nw_dp[ i - 1 ][ 0 ] + get_cost( p_w, p_P[ i - 1 ], 0, wc );
            if( p_progressCB != nullptr ) { p_progressCB( i, 0, nw_dp ); }
        }

        for( u64 i = 1; i <= m; ++i ) {
            for( u64 j = 1; j <= n; ++j ) {
                nw_dp[ i ][ j ]
                    = std::min( nw_dp[ i - 1 ][ j ] + get_cost( p_w, p_P[ i - 1 ], 0, wc ),
                                nw_dp[ i ][ j - 1 ] + get_cost( p_w, 0, p_T[ j - 1 ], wc ) );
                nw_dp[ i ][ j ] = std::min( nw_dp[ i ][ j ],
                                            nw_dp[ i - 1 ][ j - 1 ]
                                                + get_cost( p_w, p_P[ i - 1 ], p_T[ j - 1 ], wc ) );
                if( p_progressCB != nullptr ) { p_progressCB( i, j, nw_dp ); }
            }
        }

        point           pos{ m, n };
        breakpoint_repn res{ };
        res.push_front( breakpoint{ pos } );

        do {
            auto i = pos.first;
            auto j = pos.second;

            if( i && j && nw_dp[ i ][ j ] == nw_dp[ i - 1 ][ j - 1 ]
                && match( p_P[ i - 1 ], p_T[ j - 1 ], wc ) ) {
                pos = point{ i - 1, j - 1 };

                if( p_wcInOutput && ( p_P[ i - 1 ] == wc || p_T[ j - 1 ] == wc ) ) {
                    res.push_front( breakpoint{ pos,
                                                get_cost( p_w, p_P[ i - 1 ], p_T[ j - 1 ], wc ),
                                                p_P[ i - 1 ], p_T[ j - 1 ] } );
                }
            } else if( i && j
                       && nw_dp[ i ][ j ]
                              == get_cost( p_w, p_P[ i - 1 ], p_T[ j - 1 ], wc )
                                     + nw_dp[ i - 1 ][ j - 1 ] ) {
                pos = point{ i - 1, j - 1 };
                res.push_front( breakpoint{ pos, get_cost( p_w, p_P[ i - 1 ], p_T[ j - 1 ], wc ),
                                            p_P[ i - 1 ], p_T[ j - 1 ] } );
            } else if( i
                       && nw_dp[ i ][ j ]
                              == get_cost( p_w, p_P[ i - 1 ], 0, wc ) + nw_dp[ i - 1 ][ j ] ) {
                pos = point{ i - 1, j };
                res.push_front(
                    breakpoint{ pos, get_cost( p_w, p_P[ i - 1 ], 0, wc ), p_P[ i - 1 ], 0 } );
            } else if( j
                       && nw_dp[ i ][ j ]
                              == get_cost( p_w, 0, p_T[ j - 1 ], wc ) + nw_dp[ i ][ j - 1 ] ) {
                pos = point{ i, j - 1 };
                res.push_front(
                    breakpoint{ pos, get_cost( p_w, 0, p_T[ j - 1 ], wc ), 0, p_T[ j - 1 ] } );
            } else {
                fprintf( stderr, "You screwed up.\n" );
                pos = point{ 0, 0 };
            }
        } while( pos.first || pos.second );

        res.push_front( breakpoint{ pos } );

        return res;
    }

    std::deque<breakpoint_repn> compute_occs_with_mism( const std::string& p_P,
                                                        const std::string& p_T, u64 p_threshold,
                                                        const std::string& p_wildcard,
                                                        bool               p_wcInOutput ) {
        std::deque<breakpoint_repn> res{ };

        auto m  = p_P.size( );
        auto n  = p_T.size( );
        char wc = ( p_wildcard == EMPTY_STR || !p_wildcard.length( ) ) ? 0 : p_wildcard[ 0 ];

        for( u64 i = 0; i <= n - m; ++i ) {
            breakpoint_repn cur{ };
            u64             mism = 0;
            cur.push_back( breakpoint{ 0, i } );

            for( u64 j = 0; j < m && mism <= p_threshold; ++j ) {
                if( !match( p_P[ j ], p_T[ i + j ], wc ) ) {
                    cur.push_back( breakpoint{ j, i + j, p_P[ j ], p_T[ i + j ] } );
                    ++mism;
                }
                if( p_wcInOutput && ( p_P[ j ] == wc || p_T[ i + j ] == wc ) ) {
                    cur.push_back( breakpoint{ j, i + j, p_P[ j ], p_T[ i + j ] } );
                }
            }
            cur.push_back( breakpoint{ m, m + i } );

            if( mism <= p_threshold ) { res.push_back( cur ); }
        }

        return res;
    }

    std::deque<breakpoint_repn>
    compute_occs_with_edits( const std::string& p_P, const std::string& p_T, u64 p_threshold,
                             const cost_table& p_w, const std::string& p_wildcard,
                             bool p_wcInOutput, ed_dp_cb_t p_progressCB ) {
        // compute starting positions of occurrences, for each starting position one alignment
        std::deque<breakpoint_repn> ress{ };

        auto m = p_P.size( );
        auto n = p_T.size( );

        char wc = ( p_wildcard == EMPTY_STR || !p_wildcard.length( ) ) ? 0 : p_wildcard[ 0 ];

        auto nw_dp = std::deque<std::deque<u64>>{ m + 1, std::deque<u64>( n + 1, 0 ) };
        for( u64 j = 0; j <= n; ++j ) {
            nw_dp[ 0 ][ j ] = 0;
            if( p_progressCB != nullptr ) { p_progressCB( 0, j, nw_dp ); }
        }
        for( u64 i = 1; i <= m; ++i ) {
            nw_dp[ i ][ 0 ] = nw_dp[ i - 1 ][ 0 ] + get_cost( p_w, p_P[ m - i ], 0, wc );
            if( p_progressCB != nullptr ) { p_progressCB( i, 0, nw_dp ); }
        }

        for( u64 i = 1; i <= m; ++i ) {
            for( u64 j = 1; j <= n; ++j ) {
                nw_dp[ i ][ j ]
                    = std::min( nw_dp[ i - 1 ][ j ] + get_cost( p_w, p_P[ m - i ], 0, wc ),
                                nw_dp[ i ][ j - 1 ] + get_cost( p_w, 0, p_T[ n - j ], wc ) );
                nw_dp[ i ][ j ] = std::min( nw_dp[ i ][ j ],
                                            nw_dp[ i - 1 ][ j - 1 ]
                                                + get_cost( p_w, p_P[ m - i ], p_T[ n - j ], wc ) );
                if( p_progressCB != nullptr ) { p_progressCB( i, j, nw_dp ); }
            }
        }

        if( m + p_threshold > n ) { return { }; }
        for( u64 t = 0; t <= n - m + p_threshold; ++t ) {
            if( nw_dp[ m ][ n - t ] > p_threshold ) { continue; }

            point           pos{ 0, t };
            breakpoint_repn res{ };
            res.push_back( breakpoint{ pos } );

            do {
                auto i = pos.first;
                auto j = pos.second;

                auto dpi = m - i;
                auto dpj = n - j;

                if( dpi && dpj && nw_dp[ dpi ][ dpj ] == nw_dp[ dpi - 1 ][ dpj - 1 ]
                    && match( p_P[ i ], p_T[ j ], wc ) ) {
                    if( p_wcInOutput && ( p_P[ i ] == wc || p_T[ j ] == wc ) ) {
                        res.push_back( breakpoint{ pos, get_cost( p_w, p_P[ i ], p_T[ j ], wc ),
                                                   p_P[ i ], p_T[ j ] } );
                    }
                    pos = point{ i + 1, j + 1 };

                } else if( dpi && dpj
                           && nw_dp[ dpi ][ dpj ]
                                  == get_cost( p_w, p_P[ i ], p_T[ j ], wc )
                                         + nw_dp[ dpi - 1 ][ dpj - 1 ] ) {
                    res.push_back( breakpoint{ pos, get_cost( p_w, p_P[ i ], p_T[ j ], wc ),
                                               p_P[ i ], p_T[ j ] } );
                    pos = point{ i + 1, j + 1 };
                } else if( dpi
                           && nw_dp[ dpi ][ dpj ]
                                  == get_cost( p_w, p_P[ i ], 0, wc ) + nw_dp[ dpi - 1 ][ dpj ] ) {
                    res.push_back(
                        breakpoint{ pos, get_cost( p_w, p_P[ i ], 0, wc ), p_P[ i ], 0 } );
                    pos = point{ i + 1, j };
                } else if( dpj
                           && nw_dp[ dpi ][ dpj ]
                                  == get_cost( p_w, 0, p_T[ j ], wc ) + nw_dp[ dpi ][ dpj - 1 ] ) {
                    res.push_back(
                        breakpoint{ pos, get_cost( p_w, 0, p_T[ j ], wc ), 0, p_T[ j ] } );
                    pos = point{ i, j + 1 };
                } else {
                    fprintf( stderr, "You screwed up.\n" );
                    pos = point{ m, n };
                }
            } while( pos.first < m );

            res.push_back( breakpoint{ pos } );

            ress.push_back( res );
        }

        return ress;
    }

    // compute A( P[ l .. r) )
    breakpoint_repn breakpoint_slice( const breakpoint_repn& p_brpts, fragmentco p_frag ) {
        breakpoint_repn result{ };

        s32 shift = p_brpts.front( ).shift_after( );
        for( const auto& b : p_brpts ) {
            if( b.m_posP >= p_frag.m_end ) { break; }
            if( p_frag.m_begin <= b.m_posP ) {
                result.push_back( b );
            } else {
                shift = b.shift_after( );
            }
        }
        if( result.empty( ) ) {
            result.push_front(
                breakpoint{ p_frag.m_begin, static_cast<u64>( p_frag.m_begin - shift ), 0, 0 } );
        } else if( !result.front( ).is_dummy( ) ) {
            result.push_front( breakpoint{
                p_frag.m_begin,
                static_cast<u64>( p_frag.m_begin - result.front( ).shift_before( ) ), 0, 0 } );
        }
        if( result.size( ) == 1 || !result.back( ).is_dummy( ) ) {
            result.push_back( breakpoint{
                p_frag.m_end, static_cast<u64>( p_frag.m_end - result.back( ).shift_after( ) ), 0,
                0 } );
        }
        return result;
    }

    fragmentco align_fragment( const breakpoint_repn& p_brpts, fragmentco p_frag ) {
        auto sl = breakpoint_slice( p_brpts, p_frag );
        return fragmentco{ sl.front( ).m_posT, sl.back( ).m_posT };
    }

    // normalize breakpoint representation s.t. first breakpoint is at (0,0)
    breakpoint_repn breakpoint_normalize( const breakpoint_repn& p_brpts ) {
        breakpoint_repn result{ };
        for( const auto& i : p_brpts ) { result.push_back( i - p_brpts.front( ) ); }
        return result;
    }
} // namespace ALG
