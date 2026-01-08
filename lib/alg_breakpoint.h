#pragma once
#include <deque>
#include <functional>
#include <map>
#include <string>

#include "alg_fragmentco.h"
#include "defines.h"

namespace ALG {
    typedef std::pair<char, char>              edit_op;
    typedef std::map<edit_op, u64>             cost_table;
    typedef const std::deque<std::deque<u64>>& dp_table;

    typedef std::function<void( u64, u64, dp_table )> ed_dp_cb_t;

    // breakpoint of an alignment P ~>> T, as seen from P.
    struct breakpoint {
        u64  m_posP;
        u64  m_posT;
        char m_charP;
        char m_charT;
        u64  m_cost;

        breakpoint( u64 p_posP, u64 p_posT, char p_charP = 0, char p_charT = 0 )
            : m_posP{ p_posP }, m_posT{ p_posT }, m_charP{ p_charP }, m_charT{ p_charT } {
            m_cost = ( p_charP != p_charT );
        }

        breakpoint( point p_pos, char p_charP = 0, char p_charT = 0 )
            : m_posP{ p_pos.first }, m_posT{ p_pos.second }, m_charP{ p_charP },
              m_charT{ p_charT } {
            m_cost = ( p_charP != p_charT );
        }

        explicit breakpoint( u64 p_posP, u64 p_posT, u64 p_cost, char p_charP, char p_charT )
            : m_posP{ p_posP }, m_posT{ p_posT }, m_charP{ p_charP }, m_charT{ p_charT },
              m_cost{ p_cost } {
        }

        explicit breakpoint( point p_pos, u64 p_cost, char p_charP, char p_charT )
            : m_posP{ p_pos.first }, m_posT{ p_pos.second }, m_charP{ p_charP }, m_charT{ p_charT },
              m_cost{ p_cost } {
        }

        inline bool is_dummy( ) const {
            return !m_charP && !m_charT;
        }

        inline point position( ) const {
            return point{ m_posP, m_posT };
        }

        inline breakpoint operator-( const breakpoint& p_rhs ) const {
            return breakpoint{ this->m_posP - p_rhs.m_posP, this->m_posT - p_rhs.m_posT,
                               this->m_cost, this->m_charP, this->m_charT };
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

        inline breakpoint reweight( const cost_table& p_w ) const {
            auto cost = this->m_cost;
            if( p_w.count( { this->m_charP, this->m_charT } ) ) {
                cost = p_w.at( { this->m_charP, this->m_charT } );
            }
            return breakpoint{ this->m_posP, this->m_posT, cost, this->m_charP, this->m_charT };
        }
    };
    typedef std::deque<breakpoint> breakpoint_repn;

    inline breakpoint_repn reweight( const breakpoint_repn& p_bp, const cost_table& p_w ) {
        breakpoint_repn res{ };
        for( const auto& bp : p_bp ) { res.push_back( bp.reweight( p_w ) ); }
        return res;
    }

    inline breakpoint_repn trivial_alignment( fragmentco p_fragment ) {
        breakpoint_repn res{ };
        res.push_back( breakpoint{ p_fragment.closed_begin( ), p_fragment.closed_begin( ) } );
        res.push_back( breakpoint{ p_fragment.open_end( ), p_fragment.open_end( ) } );
        return res;
    }

    inline breakpoint_repn trivial_alignment( fragmentco p_fragmentP, fragmentco p_fragmentT ) {
        breakpoint_repn res{ };
        res.push_back( breakpoint{ p_fragmentP.closed_begin( ), p_fragmentT.closed_begin( ) } );
        res.push_back( breakpoint{ p_fragmentP.open_end( ), p_fragmentT.open_end( ) } );
        return res;
    }

    breakpoint_repn compute_breakpoints_mism( const std::string& p_P, const std::string& p_T,
                                              const std::string& p_wildcard   = WILDCARD,
                                              bool               p_wcInOutput = true );

    breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T,
                                         const cost_table&  p_w,
                                         const std::string& p_wildcard   = WILDCARD,
                                         bool               p_wcInOutput = true,
                                         ed_dp_cb_t         p_progressCB = nullptr );

    // computes optimal (unweighted) edit distance alignment of P onto T
    inline breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T,
                                                const std::string& p_wildcard   = WILDCARD,
                                                bool               p_wcInOutput = true,
                                                ed_dp_cb_t         p_progressCB = nullptr ) {
        return compute_breakpoints( p_P, p_T, { }, p_wildcard, p_wcInOutput, p_progressCB );
    }

    std::deque<breakpoint_repn>
    compute_occs_with_edits( const std::string& p_P, const std::string& p_T, u64 p_threshold,
                             const cost_table& p_w, const std::string& p_wildcard = WILDCARD,
                             bool p_wcInOutput = true, ed_dp_cb_t p_progressCB = nullptr );

    std::deque<breakpoint_repn>
    compute_occs_with_edits_lv( const std::string& p_P, const std::string& p_T, u64 p_threshold,
                                const std::string& p_wildcard = WILDCARD, bool p_wcInOutput = true,
                                ed_dp_cb_t p_progressCB = nullptr );

    inline std::deque<breakpoint_repn>
    compute_occs_with_edits( const std::string& p_P, const std::string& p_T, u64 p_threshold,
                             const std::string& p_wildcard = WILDCARD, bool p_wcInOutput = true,
                             ed_dp_cb_t p_progressCB = nullptr ) {
        return compute_occs_with_edits( p_P, p_T, p_threshold, { }, p_wildcard, p_wcInOutput,
                                        p_progressCB );
    }

    std::deque<breakpoint_repn> compute_occs_with_mism( const std::string& p_P,
                                                        const std::string& p_T, u64 p_threshold,
                                                        const std::string& p_wildcard   = WILDCARD,
                                                        bool               p_wcInOutput = true );

    inline std::deque<breakpoint_repn> compute_occs( const std::string& p_P, const std::string& p_T,
                                                     const std::string& p_wildcard   = WILDCARD,
                                                     bool               p_wcInOutput = true ) {
        return compute_occs_with_mism( p_P, p_T, 0, p_wildcard, p_wcInOutput );
    }

    // compute A( P[ l .. r) )
    breakpoint_repn breakpoint_slice( const breakpoint_repn& p_brpts, fragmentco p_frag );

    fragmentco align_fragment( const breakpoint_repn& p_brpts, fragmentco p_frag );

    // normalize breakpoint representation s.t. first breakpoint is at (0,0)
    breakpoint_repn breakpoint_normalize( const breakpoint_repn& p_brpts );

    inline std::string substitute_wildcards( const std::string& p_wcString,
                                             const std::string& p_wcSubst,
                                             const std::string& p_wildcard = WILDCARD ) {
        std::string res = "";
        u64         j   = 0;
        for( u64 i = 0; i < p_wcString.size( ); ++i ) {
            if( std::string{ p_wcString[ i ] } == p_wildcard ) {
                res += p_wcSubst[ j++ ];
            } else {
                res += p_wcString[ i ];
            }
        }
        return res;
    }

} // namespace ALG
