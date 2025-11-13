#pragma once

#include <deque>

#include "breakpoint.h"
#include "tikz.h"

enum class gs_flag_t : u32 {
    GSF_NONE      = 0,
    GSF_PURE      = ( 1 << 0 ),
    GSF_TOP       = ( 1 << 1 ),
    GSF_BOT       = ( 1 << 2 ),
    GSF_PRIMITIVE = ( 1 << 3 ),
};

constexpr bool operator!( gs_flag_t p_a ) {
    return !static_cast<u32>( p_a );
}
constexpr gs_flag_t operator|( gs_flag_t p_a, gs_flag_t p_b ) {
    return static_cast<gs_flag_t>( static_cast<u32>( p_a ) | static_cast<u32>( p_b ) );
}
constexpr gs_flag_t operator&( gs_flag_t p_a, gs_flag_t p_b ) {
    return static_cast<gs_flag_t>( static_cast<u32>( p_a ) & static_cast<u32>( p_b ) );
}
constexpr gs_flag_t operator^( gs_flag_t p_a, gs_flag_t p_b ) {
    return static_cast<gs_flag_t>( static_cast<u32>( p_a ) ^ static_cast<u32>( p_b ) );
}
inline gs_flag_t& operator|=( gs_flag_t& p_a, gs_flag_t p_b ) {
    return (gs_flag_t&) ( (u32&) ( p_a ) |= static_cast<u32>( p_b ) );
}
inline gs_flag_t& operator&=( gs_flag_t& p_a, gs_flag_t p_b ) {
    return (gs_flag_t&) ( (u32&) ( p_a ) &= static_cast<u32>( p_b ) );
}
inline gs_flag_t& operator^=( gs_flag_t& p_a, gs_flag_t p_b ) {
    return (gs_flag_t&) ( (u32&) ( p_a ) ^= static_cast<u32>( p_b ) );
}

gs_flag_t merge_flags( gs_flag_t p_a, gs_flag_t p_b );

struct graph_slice {
    gs_flag_t  m_flags;
    fragmentco m_originalFragP;

    // points are stored in (x , y), that is, (posT, posP)
    std::deque<point> m_topFrontier;    // left-to-right
    std::deque<point> m_bottomFrontier; // right-to-left

    std::deque<point> m_leftPortals;  // top-to-bottom
    std::deque<point> m_rightPortals; // top-to-bottom

    std::deque<point> m_innerPortals; // column-wise top-to-bottom; columns left-to-right

    inline point top_left( ) const {
        return m_topFrontier.front( );
    }

    inline point bottom_right( ) const {
        return m_bottomFrontier.front( );
    }

    inline fragmentco fragment_p( ) const {
        return fragmentco{ top_left( ).second, bottom_right( ).second };
    }

    inline fragmentco original_fragment_p( ) const {
        return m_originalFragP;
    }

    inline fragmentco fragment_t( ) const {
        return fragmentco{ top_left( ).first, bottom_right( ).first };
    }

    inline u32 width( ) const {
        return fragment_t( ).length( );
    }

    inline u32 height( ) const {
        return fragment_p( ).length( );
    }

    void merge_left( const graph_slice& p_other );
    void merge_right( const graph_slice& p_other );

    static std::deque<graph_slice> from_alignment( const breakpoint_repn& p_brpnt, u32 p_d );
    static graph_slice             merged_slice( const std::deque<graph_slice>& p_primitiveSlices,
                                                 fragmentco                     p_fragment );
    static std::deque<graph_slice>
    top_pure_bot_decomp( const std::deque<graph_slice>& p_primitiveSlices );
};
