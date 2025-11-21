#pragma once

#include <deque>
#include <string>

#include "defines.h"
#include "fragmentco.h"

// breakpoint of an alignment P ~>> T, as seen from P.
struct breakpoint {
    u64  m_posP;
    u64  m_posT;
    char m_charP;
    char m_charT;

    breakpoint( u64 p_posP, u64 p_posT, char p_charP = 0, char p_charT = 0 )
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

// computes optimal (unweighted) edit distance alignment of P onto T
breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T,
                                     const std::string& p_wildcard   = WILDCARD,
                                     bool               p_wcInOutput = true );

std::deque<breakpoint_repn> compute_occs_with_edits( const std::string& p_P, const std::string& p_T,
                                                     u64                p_threshold,
                                                     const std::string& p_wildcard   = WILDCARD,
                                                     bool               p_wcInOutput = true );

std::deque<breakpoint_repn> compute_occs_with_mism( const std::string& p_P, const std::string& p_T,
                                                    u64                p_threshold,
                                                    const std::string& p_wildcard   = WILDCARD,
                                                    bool               p_wcInOutput = true );

inline std::deque<breakpoint_repn> compute_occs( const std::string& p_P, const std::string& p_T,
                                                 const std::string& p_wildcard = WILDCARD ) {
    return compute_occs_with_mism( p_P, p_T, 0, p_wildcard );
}

// compute A( P[ l .. r) )
breakpoint_repn breakpoint_slice( const breakpoint_repn& p_brpts, fragmentco p_frag,
                                  bool p_print = false );

fragmentco align_fragment( const breakpoint_repn& p_brpts, fragmentco p_frag,
                           bool p_print = false );

// normalize breakpoint representation s.t. first breakpoint is at (0,0)
breakpoint_repn breakpoint_normalize( const breakpoint_repn& p_brpts );
