#include "alg_ag_slice.h"

namespace ALG {
    point graph_slice::top_left( ) const {
        return m_topFrontier.front( );
    }

    point graph_slice::bottom_right( ) const {
        return m_bottomFrontier.front( );
    }

    fragmentco graph_slice::fragment_p( ) const {
        return fragmentco{ top_left( ).second, bottom_right( ).second };
    }

    fragmentco graph_slice::original_fragment_p( ) const {
        return m_originalFragP;
    }

    fragmentco graph_slice::fragment_t( ) const {
        return fragmentco{ top_left( ).first, bottom_right( ).first };
    }

    u64 graph_slice::width( ) const {
        return fragment_t( ).length( );
    }

    u64 graph_slice::height( ) const {
        return fragment_p( ).length( );
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
        gs_flag_t res = graph_slice::GSF_NONE;

        if( !!( p_a & graph_slice::GSF_PURE ) && !!( p_b & graph_slice::GSF_PURE ) ) {
            res |= graph_slice::GSF_PURE;
        }
        if( !!( p_a & graph_slice::GSF_TOP ) && !!( p_b & graph_slice::GSF_TOP ) ) {
            res |= graph_slice::GSF_TOP;
        }
        if( !!( p_a & graph_slice::GSF_BOT ) && !!( p_b & graph_slice::GSF_BOT ) ) {
            res |= graph_slice::GSF_BOT;
        }

        return res;
    }

    std::deque<graph_slice> graph_slice::from_alignment( const breakpoint_repn& p_brpnt, u64 p_d ) {
        fragmentco              fragP{ p_brpnt.front( ).m_posP, p_brpnt.back( ).m_posP };
        std::deque<graph_slice> res{ };

        for( u64 i = fragP.closed_begin( ); i < fragP.open_end( ); ++i ) {
            auto        fP  = fragP.d_slice( p_d, i );
            auto        fpi = fragmentco{ i, i + 1 };
            auto        fT  = align_fragment( p_brpnt, fpi );
            graph_slice gi{ };
            gi.m_originalFragP = fpi;

            gi.m_flags = graph_slice::GSF_PRIMITIVE;
            if( fP.closed_begin( ) == fragP.closed_begin( ) ) {
                gi.m_flags |= graph_slice::GSF_TOP;
            }
            if( fP.open_end( ) == fragP.open_end( ) ) { gi.m_flags |= graph_slice::GSF_BOT; }

            // slice is pure if p_brpnt is trivial on fragmentco{ i, i + 1 }, that is,
            // contains no edit operations and only the dummy entries at start/end
            if( breakpoint_slice( p_brpnt, fragmentco{ i, i + 1 } ).size( ) == 2 ) {
                gi.m_flags |= graph_slice::GSF_PURE;
            }

            gi.m_topFrontier.push_back( { fT.closed_begin( ), fP.closed_begin( ) } );
            gi.m_topFrontier.push_back( { fT.open_end( ), fP.closed_begin( ) } );

            gi.m_bottomFrontier.push_back( { fT.open_end( ), fP.open_end( ) } );
            gi.m_bottomFrontier.push_back( { fT.closed_begin( ), fP.open_end( ) } );

            if( i ) {
                auto vb = fP.closed_begin( );
                auto ve = fragP.d_slice( p_d, i - 1 ).open_end( );
                for( u64 y = vb; y <= ve; ++y ) {
                    gi.m_leftPortals.push_back( { fT.closed_begin( ), y } );
                }
            }
            if( i < fragP.closed_end( ) ) {
                auto vb = fragP.d_slice( p_d, i + 1 ).closed_begin( );
                auto ve = fP.open_end( );
                for( u64 y = vb; y <= ve; ++y ) {
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
        for( u64 i = p_fragment.open_begin( ); i < p_fragment.open_end( ); ++i ) {
            res.merge_right( p_primitiveSlices[ i ] );
        }
        return res;
    }

    std::deque<graph_slice>
    graph_slice::top_pure_bot_decomp( const std::deque<graph_slice>& p_primitiveSlices ) {
        std::deque<graph_slice> res;

        u64 pos = 0;
        if( !!( p_primitiveSlices.front( ).m_flags & graph_slice::GSF_TOP ) ) {
            graph_slice top = p_primitiveSlices.front( );
            while( ++pos < p_primitiveSlices.size( )
                   && !!( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_TOP ) ) {
                top.merge_right( p_primitiveSlices[ pos ] );
            }
            res.push_back( top );
        }

        while( pos < p_primitiveSlices.size( )
               && !( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_BOT ) ) {
            if( !!( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_PURE ) ) {
                graph_slice pure = p_primitiveSlices[ pos ];
                while( ++pos < p_primitiveSlices.size( )
                       && !( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_BOT )
                       && !!( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_PURE ) ) {
                    pure.merge_right( p_primitiveSlices[ pos ] );
                }
                res.push_back( pure );
            } else {
                graph_slice impure = p_primitiveSlices[ pos ];
                while( ++pos < p_primitiveSlices.size( )
                       && !( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_BOT )
                       && !( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_PURE ) ) {
                    impure.merge_right( p_primitiveSlices[ pos ] );
                }
                res.push_back( impure );
            }
        }

        if( pos < p_primitiveSlices.size( )
            && !!( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_BOT ) ) {
            graph_slice bot = p_primitiveSlices[ pos ];
            while( ++pos < p_primitiveSlices.size( )
                   && !!( p_primitiveSlices[ pos ].m_flags & graph_slice::GSF_BOT ) ) {
                bot.merge_right( p_primitiveSlices[ pos ] );
            }
            res.push_back( bot );
        }

        return res;
    }
} // namespace ALG
