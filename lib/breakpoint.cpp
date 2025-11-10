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

void graph_slice::merge_left( const graph_slice& p_other ) {
    m_topFrontier.prepend_range( p_other.m_topFrontier );
    m_bottomFrontier.append_range( p_other.m_bottomFrontier );

    m_innerPortals.prepend_range( m_leftPortals );
    m_innerPortals.prepend_range( p_other.m_innerPortals );

    m_leftPortals = p_other.m_leftPortals;

    m_flags = merge_flags( m_flags, p_other.m_flags );

    m_originalFragP
        = fragmentco{ p_other.m_originalFragP.closed_begin( ), m_originalFragP.open_end( ) };
}

void graph_slice::merge_right( const graph_slice& p_other ) {
    m_topFrontier.append_range( p_other.m_topFrontier );
    m_bottomFrontier.prepend_range( p_other.m_bottomFrontier );

    m_innerPortals.append_range( m_rightPortals );
    m_innerPortals.append_range( p_other.m_innerPortals );

    m_rightPortals = p_other.m_rightPortals;

    m_flags = merge_flags( m_flags, p_other.m_flags );

    m_originalFragP
        = fragmentco{ m_originalFragP.closed_begin( ), p_other.m_originalFragP.open_end( ) };
}

gs_flag_t merge_flags( gs_flag_t p_a, gs_flag_t p_b ) {
    gs_flag_t res = gs_flag_t::GSF_NONE;

    if( !!( p_a & gs_flag_t::GSF_PURE ) && !!( p_b & gs_flag_t::GSF_PURE ) ) {
        res |= gs_flag_t::GSF_PURE;
    }
    if( !!( p_a & gs_flag_t::GSF_TOP ) && !!( p_b & gs_flag_t::GSF_TOP ) ) {
        res |= gs_flag_t::GSF_TOP;
    }
    if( !!( p_a & gs_flag_t::GSF_BOT ) && !!( p_b & gs_flag_t::GSF_BOT ) ) {
        res |= gs_flag_t::GSF_BOT;
    }

    return res;
}

std::deque<graph_slice> graph_slice::from_alignment( const breakpoint_repn& p_brpnt, u32 p_d ) {
    fragmentco              fragP{ p_brpnt.front( ).m_posP, p_brpnt.back( ).m_posP };
    std::deque<graph_slice> res{ };

    for( u32 i = fragP.closed_begin( ); i < fragP.open_end( ); ++i ) {
        auto        fP  = fragP.d_slice( p_d, i );
        auto        fpi = fragmentco{ i, i + 1 };
        auto        fT  = align_fragment( p_brpnt, fpi );
        graph_slice gi{ };
        gi.m_originalFragP = fpi;

        gi.m_flags = gs_flag_t::GSF_PRIMITIVE;
        if( fP.closed_begin( ) == fragP.closed_begin( ) ) { gi.m_flags |= gs_flag_t::GSF_TOP; }
        if( fP.open_end( ) == fragP.open_end( ) ) { gi.m_flags |= gs_flag_t::GSF_BOT; }

        // slice is pure if p_brpnt is trivial on fragmentco{ i, i + 1 }, that is,
        // contains no edit operations and only the dummy entries at start/end
        if( breakpoint_slice( p_brpnt, fragmentco{ i, i + 1 } ).size( ) == 2 ) {
            gi.m_flags |= gs_flag_t::GSF_PURE;
        }

        gi.m_topFrontier.push_back( { fT.closed_begin( ), fP.closed_begin( ) } );
        gi.m_topFrontier.push_back( { fT.open_end( ), fP.closed_begin( ) } );

        gi.m_bottomFrontier.push_back( { fT.open_end( ), fP.open_end( ) } );
        gi.m_bottomFrontier.push_back( { fT.closed_begin( ), fP.open_end( ) } );

        if( i ) {
            auto vb = fP.closed_begin( );
            auto ve = fragP.d_slice( p_d, i - 1 ).open_end( );
            for( u32 y = vb; y <= ve; ++y ) {
                gi.m_leftPortals.push_back( { fT.closed_begin( ), y } );
            }
        }
        if( i < fragP.closed_end( ) ) {
            auto vb = fragP.d_slice( p_d, i + 1 ).closed_begin( );
            auto ve = fP.open_end( );
            for( u32 y = vb; y <= ve; ++y ) {
                gi.m_rightPortals.push_back( { fT.open_end( ), y } );
            }
        }

        res.push_back( gi );
    }
    return res;
}

graph_slice graph_slice::merged_slice( const std::deque<graph_slice>& p_primitiveSlices,
                                       fragmentco                     p_fragment ) {
    graph_slice res = p_primitiveSlices[ p_fragment.closed_begin( ) ];
    for( u32 i = p_fragment.open_begin( ); i < p_fragment.open_end( ); ++i ) {
        res.merge_right( p_primitiveSlices[ i ] );
    }
    return res;
}

std::deque<graph_slice>
graph_slice::top_pure_bot_decomp( const std::deque<graph_slice>& p_primitiveSlices ) {
    std::deque<graph_slice> res;

    u32 pos = 0;
    if( !!( p_primitiveSlices.front( ).m_flags & gs_flag_t::GSF_TOP ) ) {
        graph_slice top = p_primitiveSlices.front( );
        while( ++pos < p_primitiveSlices.size( )
               && !!( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_TOP ) ) {
            top.merge_right( p_primitiveSlices[ pos ] );
        }
        res.push_back( top );
    }

    while( pos < p_primitiveSlices.size( )
           && !( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_BOT ) ) {
        if( !!( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_PURE ) ) {
            graph_slice pure = p_primitiveSlices[ pos ];
            while( ++pos < p_primitiveSlices.size( )
                   && !( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_BOT )
                   && !!( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_PURE ) ) {
                pure.merge_right( p_primitiveSlices[ pos ] );
            }
            res.push_back( pure );
        } else {
            graph_slice impure = p_primitiveSlices[ pos ];
            while( ++pos < p_primitiveSlices.size( )
                   && !( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_BOT )
                   && !( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_PURE ) ) {
                impure.merge_right( p_primitiveSlices[ pos ] );
            }
            res.push_back( impure );
        }
    }

    if( pos < p_primitiveSlices.size( )
        && !!( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_BOT ) ) {
        graph_slice bot = p_primitiveSlices[ pos ];
        while( ++pos < p_primitiveSlices.size( )
               && !!( p_primitiveSlices[ pos ].m_flags & gs_flag_t::GSF_BOT ) ) {
            bot.merge_right( p_primitiveSlices[ pos ] );
        }
        res.push_back( bot );
    }

    return res;
}
