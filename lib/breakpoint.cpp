#include "breakpoint.h"

bool match( char p_P, char p_T, char p_wc ) {
    if( !p_wc ) {
        return p_P == p_T;
    } else {
        return p_P == p_wc || p_T == p_wc || p_P == p_T;
    }
}

breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T,
                                     const std::string& p_wildcard, bool p_wcInOutput ) {
    auto m = p_P.size( );
    auto n = p_T.size( );

    char wc = ( p_wildcard == EMPTY_STR || !p_wildcard.length( ) ) ? 0 : p_wildcard[ 0 ];

    auto nw_dp = std::deque<std::deque<u64>>{ m + 1, std::deque<u64>( n + 1, 0 ) };
    for( u64 i = 0; i <= m; ++i ) { nw_dp[ i ][ 0 ] = i; }
    for( u64 j = 0; j <= n; ++j ) { nw_dp[ 0 ][ j ] = j; }

    for( u64 i = 1; i <= m; ++i ) {
        for( u64 j = 1; j <= n; ++j ) {
            nw_dp[ i ][ j ] = std::min( nw_dp[ i - 1 ][ j ] + 1, nw_dp[ i ][ j - 1 ] + 1 );
            if( match( p_P[ i - 1 ], p_T[ j - 1 ], wc ) ) {
                nw_dp[ i ][ j ] = std::min( nw_dp[ i ][ j ], nw_dp[ i - 1 ][ j - 1 ] );
            } else {
                nw_dp[ i ][ j ] = std::min( nw_dp[ i ][ j ], nw_dp[ i - 1 ][ j - 1 ] + 1 );
            }
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
                res.push_front( breakpoint{ pos, p_P[ i - 1 ], p_T[ j - 1 ] } );
            }
        } else if( i && j && nw_dp[ i ][ j ] == 1 + nw_dp[ i - 1 ][ j - 1 ] ) {
            pos = point{ i - 1, j - 1 };
            res.push_front( breakpoint{ pos, p_P[ i - 1 ], p_T[ j - 1 ] } );
        } else if( i && nw_dp[ i ][ j ] == 1 + nw_dp[ i - 1 ][ j ] ) {
            pos = point{ i - 1, j };
            res.push_front( breakpoint{ pos, p_P[ i - 1 ], 0 } );
        } else if( j && nw_dp[ i ][ j ] == 1 + nw_dp[ i ][ j - 1 ] ) {
            pos = point{ i, j - 1 };
            res.push_front( breakpoint{ pos, 0, p_T[ j - 1 ] } );
        } else {
            fprintf( stderr, "You screwed up.\n" );
            pos = point{ 0, 0 };
        }
    } while( pos.first || pos.second );

    res.push_front( breakpoint{ pos } );

    return res;
}

std::deque<breakpoint_repn> compute_occs_with_mism( const std::string& p_P, const std::string& p_T,
                                                    u64 p_threshold, const std::string& p_wildcard,
                                                    bool p_wcInOutput ) {
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

std::deque<breakpoint_repn> compute_occs_with_edits( const std::string& p_P, const std::string& p_T,
                                                     u64 p_threshold, const std::string& p_wildcard,
                                                     bool p_wcInOutput ) {
    // compute starting positions of occurrences, for each starting position one alignment
    std::deque<breakpoint_repn> ress{ };

    auto m  = p_P.size( );
    auto n  = p_T.size( );
    char wc = ( p_wildcard == EMPTY_STR || !p_wildcard.length( ) ) ? 0 : p_wildcard[ 0 ];

    auto nw_dp = std::deque<std::deque<u64>>{ m + 1, std::deque<u64>( n + 1, 0 ) };
    for( u64 i = 0; i <= m; ++i ) { nw_dp[ i ][ 0 ] = i; }
    for( u64 j = 0; j <= n; ++j ) { nw_dp[ 0 ][ j ] = 0; }

    for( u64 i = 1; i <= m; ++i ) {
        for( u64 j = 1; j <= n; ++j ) {
            nw_dp[ i ][ j ] = std::min( nw_dp[ i - 1 ][ j ] + 1, nw_dp[ i ][ j - 1 ] + 1 );
            if( match( p_P[ m - i ], p_T[ n - j ], wc ) ) {
                nw_dp[ i ][ j ] = std::min( nw_dp[ i ][ j ], nw_dp[ i - 1 ][ j - 1 ] );
            } else {
                nw_dp[ i ][ j ] = std::min( nw_dp[ i ][ j ], nw_dp[ i - 1 ][ j - 1 ] + 1 );
            }
        }
    }

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
                    res.push_back( breakpoint{ pos, p_P[ i ], p_T[ j ] } );
                }
                pos = point{ i + 1, j + 1 };

            } else if( dpi && dpj && nw_dp[ dpi ][ dpj ] == 1 + nw_dp[ dpi - 1 ][ dpj - 1 ] ) {
                res.push_back( breakpoint{ pos, p_P[ i ], p_T[ j ] } );
                pos = point{ i + 1, j + 1 };
            } else if( dpi && nw_dp[ dpi ][ dpj ] == 1 + nw_dp[ dpi - 1 ][ dpj ] ) {
                res.push_back( breakpoint{ pos, p_P[ i ], 0 } );
                pos = point{ i + 1, j };
            } else if( dpj && nw_dp[ dpi ][ dpj ] == 1 + nw_dp[ dpi ][ dpj - 1 ] ) {
                res.push_back( breakpoint{ pos, 0, p_T[ j ] } );
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
            p_frag.m_begin, static_cast<u64>( p_frag.m_begin - result.front( ).shift_before( ) ), 0,
            0 } );
    }
    if( result.size( ) == 1 || !result.back( ).is_dummy( ) ) {
        result.push_back(
            breakpoint{ p_frag.m_end,
                        static_cast<u64>( p_frag.m_end - result.back( ).shift_after( ) ), 0, 0 } );
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
