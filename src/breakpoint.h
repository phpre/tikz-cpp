#pragma once

#include <deque>
#include <string>

#include "tikz.h"

struct fragmentco {
    u32 m_begin; // inclusive
    u32 m_end;   // exclusive

    inline u32 length( ) const {
        if( m_end >= m_begin ) {
            return m_end - m_begin;
        } else {
            return 0;
        }
    }
    inline u32 closed_end( ) const {
        return m_end - 1;
    }
    inline u32 open_end( ) const {
        return m_end;
    }
    inline u32 closed_begin( ) const {
        return m_begin;
    }
    inline u32 open_begin( ) const {
        return m_begin + 1;
    }

    inline fragmentco d_slice( u32 p_d, u32 p_i ) const {
        auto i{ p_i };
        if( i <= 2 * p_d ) {
            i = 0;
        } else {
            i = p_i - 2 * p_d;
        }
        return { std::max( this->closed_begin( ), i ),
                 std::min( this->open_end( ), p_i + 2 * p_d + 1 ) };
    }
};

// breakpoint of an alignment P ~>> T, as seen from P.
struct breakpoint {
    u32  m_posP;
    u32  m_posT;
    char m_charP;
    char m_charT;

    breakpoint( u32 p_posP, u32 p_posT, char p_charP = 0, char p_charT = 0 )
        : m_posP{ p_posP }, m_posT{ p_posT }, m_charP{ p_charP }, m_charT{ p_charT } {
    }

    breakpoint( point p_pos, char p_charP = 0, char p_charT = 0 )
        : m_posP{ p_pos.first }, m_posT{ p_pos.second }, m_charP{ p_charP }, m_charT{ p_charT } {
    }

    inline bool is_dummy( ) const {
        return !m_charP && !m_charT;
    }

    inline point position( ) const {
        return point{ m_posP, m_posT };
    }

    inline breakpoint operator-( const breakpoint& p_rhs ) const {
        return breakpoint{ this->m_posP - p_rhs.m_posP, this->m_posT - p_rhs.m_posT, this->m_charP,
                           this->m_charT };
    }

    inline long shift( ) const {
        return ( !!m_charP ) - ( !!m_charT );
    }

    inline long shift_before( ) const {
        return m_posP - m_posT;
    }

    inline long shift_after( ) const {
        return shift_before( ) + shift( );
    }
};

typedef std::deque<breakpoint> breakpoint_repn;

inline breakpoint_repn trivial_alignment( fragmentco p_fragment ) {
    breakpoint_repn res{ };
    res.push_back( breakpoint{ p_fragment.closed_begin( ), p_fragment.closed_begin( ) } );
    res.push_back( breakpoint{ p_fragment.open_end( ), p_fragment.open_end( ) } );
    return res;
}

breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T );

// compute A( P[ l .. r) )
breakpoint_repn breakpoint_slice( const breakpoint_repn& p_brpts, fragmentco p_frag,
                                  bool p_print = false );

fragmentco align_fragment( const breakpoint_repn& p_brpts, fragmentco p_frag,
                           bool p_print = false );

// normalize breakpoint representation s.t. first breakpoint is at (0,0)
breakpoint_repn breakpoint_normalize( const breakpoint_repn& p_brpts );

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
