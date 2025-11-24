#pragma once
#include <deque>
#include "alg_breakpoint.h"
#include "alg_fragmentco.h"
#include "defines.h"

namespace ALG {
    typedef u64 gs_flag_t;
    gs_flag_t   merge_flags( gs_flag_t p_a, gs_flag_t p_b );

    struct graph_slice {
        static constexpr gs_flag_t GSF_NONE      = 0;
        static constexpr gs_flag_t GSF_PURE      = ( 1 << 0 );
        static constexpr gs_flag_t GSF_TOP       = ( 1 << 1 );
        static constexpr gs_flag_t GSF_BOT       = ( 1 << 2 );
        static constexpr gs_flag_t GSF_PRIMITIVE = ( 1 << 3 );

        gs_flag_t  m_flags;
        fragmentco m_originalFragP;

        // points are stored in (x , y), that is, (posT, posP)
        std::deque<point> m_topFrontier;    // left-to-right
        std::deque<point> m_bottomFrontier; // right-to-left

        std::deque<point> m_leftPortals;  // top-to-bottom
        std::deque<point> m_rightPortals; // top-to-bottom

        std::deque<point> m_innerPortals; // column-wise top-to-bottom; columns left-to-right

        point top_left( ) const;
        point bottom_right( ) const;

        fragmentco fragment_p( ) const;
        fragmentco original_fragment_p( ) const;
        fragmentco fragment_t( ) const;

        u64 width( ) const;
        u64 height( ) const;

        void merge_left( const graph_slice& p_other );
        void merge_right( const graph_slice& p_other );

        static std::deque<graph_slice> from_alignment( const breakpoint_repn& p_brpnt, u64 p_d );
        static graph_slice merged_slice( const std::deque<graph_slice>& p_primitiveSlices,
                                         fragmentco                     p_fragment );
        static std::deque<graph_slice>
        top_pure_bot_decomp( const std::deque<graph_slice>& p_primitiveSlices );
    };
} // namespace ALG
