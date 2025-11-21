#pragma once

#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz.h"

namespace TIKZ {
    struct kv_store;

    struct tikz_option {
        std::string m_name;
        std::string m_defaultValue = EMPTY_STR;
        std::string m_value        = EMPTY_STR;

        std::set<std::string>              m_libraries = { }; // dependencies
        std::set<std::string>              m_packages  = { }; // dependencies
        std::map<std::string, std::string> m_styles    = { }; // dependencies

        std::string to_string( ) const;
        tikz_option operator( )( const std::string& p_value = EMPTY_STR ) const;
        kv_store    operator|( const tikz_option& p_other ) const;
                    operator kv_store( ) const;
    };

    struct kv_store {
        std::deque<tikz_option> m_options = { };

        inline bool empty( ) const {
            return m_options.empty( );
        }

        inline std::string to_string( ) const {
            std::string res = "";
            for( u64 i = 0; i < m_options.size( ); ++i ) {
                res += m_options[ i ].to_string( );
                if( i + 1 < m_options.size( ) ) { res += ", "; }
            }
            return res;
        }

        inline kv_store operator|( const tikz_option& p_other ) const {
            kv_store res{ };
            res.m_options.append_range( m_options );
            res.m_options.push_back( p_other );
            return res;
        }

        inline std::set<std::string> libraries( ) const {
            std::set<std::string> res{ };
            for( const auto& op : m_options ) { res.insert_range( op.m_libraries ); }
            return res;
        }

        inline std::set<std::string> packages( ) const {
            std::set<std::string> res{ };
            for( const auto& op : m_options ) { res.insert_range( op.m_packages ); }
            return res;
        }

        inline std::map<std::string, std::string> styles( ) const {
            std::map<std::string, std::string> res{ };
            for( const auto& op : m_options ) { res.insert_range( op.m_styles ); }
            return res;
        }
    };

    namespace OPT {
        extern const tikz_option TEXT_COLOR;
        extern const tikz_option DRAW;
        extern const tikz_option FILL;

        extern const tikz_option ROTATE;
        extern const tikz_option XSCALE;
        extern const tikz_option YSCALE;

        extern const tikz_option OUTER_SEP;
        extern const tikz_option INNER_SEP;

        extern const tikz_option ROUNDED_CORNERS;

        extern const tikz_option LINE_WIDTH;
        extern const tikz_option LW_VERY_THIN_OUTLINE;
        extern const tikz_option LW_THIN_OUTLINE;
        extern const tikz_option LW_OUTLINE;
        extern const tikz_option LW_SUPPORT_LINE;
        extern const tikz_option LW_THICK_SUPPORT_LINE;

        extern const tikz_option ARR_TIP_LATEX;
    } // namespace OPT
} // namespace TIKZ
