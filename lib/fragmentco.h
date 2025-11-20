#pragma once
#include "defines.h"

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

