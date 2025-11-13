#include <vector>

#include "breakpoint.h"

breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T ) {
    auto m = p_P.size( );
    auto n = p_T.size( );

    auto nw_dp = std::vector<std::vector<u32>>{ m + 1, std::vector<u32>( n + 1, 0 ) };
    for( u32 i = 0; i <= m; ++i ) { nw_dp[ i ][ 0 ] = i; }
    for( u32 j = 0; j <= n; ++j ) { nw_dp[ 0 ][ j ] = j; }

    for( u32 i = 1; i <= m; ++i ) {
        for( u32 j = 1; j <= n; ++j ) {
            nw_dp[ i ][ j ] = std::min( nw_dp[ i - 1 ][ j ] + 1, nw_dp[ i ][ j - 1 ] + 1 );
            if( p_P[ i - 1 ] == p_T[ j - 1 ] ) {
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

        if( i && j && nw_dp[ i ][ j ] == nw_dp[ i - 1 ][ j - 1 ] && p_P[ i - 1 ] == p_T[ j - 1 ] ) {
            pos = point{ i - 1, j - 1 };
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

// compute A( P[ l .. r) )
breakpoint_repn breakpoint_slice( const breakpoint_repn& p_brpts, fragmentco p_frag,
                                  bool p_print ) {
    for( auto i : p_brpts ) {
        PRINT_MACRO( "(%i %c, %i %c) ", i.m_posP, i.m_charP, i.m_posT, i.m_charT );
    }
    PRINT_MACRO( "\n" );

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
            breakpoint{ p_frag.m_begin, static_cast<u32>( p_frag.m_begin - shift ), 0, 0 } );
    } else if( !result.front( ).is_dummy( ) ) {
        result.push_front( breakpoint{
            p_frag.m_begin, static_cast<u32>( p_frag.m_begin - result.front( ).shift_before( ) ), 0,
            0 } );
    }
    if( result.size( ) == 1 || !result.back( ).is_dummy( ) ) {
        result.push_back(
            breakpoint{ p_frag.m_end,
                        static_cast<u32>( p_frag.m_end - result.back( ).shift_after( ) ), 0, 0 } );
    }

    for( auto i : result ) {
        PRINT_MACRO( "(%i %c, %i %c) ", i.m_posP, i.m_charP, i.m_posT, i.m_charT );
    }
    PRINT_MACRO( "\n" );

    return result;
}

fragmentco align_fragment( const breakpoint_repn& p_brpts, fragmentco p_frag, bool p_print ) {
    auto sl = breakpoint_slice( p_brpts, p_frag );
    PRINT_MACRO( "  " );
    for( auto i : sl ) {
        PRINT_MACRO( "(%i %c, %i %c) ", i.m_posP, i.m_charP, i.m_posT, i.m_charT );
    }
    PRINT_MACRO( "\n" );

    return fragmentco{ sl.front( ).m_posT, sl.back( ).m_posT };
}

// normalize breakpoint representation s.t. first breakpoint is at (0,0)
breakpoint_repn breakpoint_normalize( const breakpoint_repn& p_brpts ) {
    breakpoint_repn result{ };
    for( const auto& i : p_brpts ) { result.push_back( i - p_brpts.front( ) ); }
    return result;
}
