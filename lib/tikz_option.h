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
        std::string m_defaultValue;
        std::string m_value;

        std::set<std::string> m_libraries; // dependencies
        std::set<std::string> m_packages;  // dependencies

        tikz_option( const std::string& p_name, const std::string& p_defaultValue = EMPTY_STR,
                     const std::string&           p_value     = EMPTY_STR,
                     const std::set<std::string>& p_libraries = { },
                     const std::set<std::string>& p_packages  = { } )
            : m_name{ p_name }, m_defaultValue{ p_defaultValue },
              m_value{ p_value == EMPTY_STR ? p_defaultValue : p_value },
              m_libraries{ p_libraries.begin( ), p_libraries.end( ) },
              m_packages{ p_packages.begin( ), p_packages.end( ) } {
        }

        std::string to_string( ) const;
        tikz_option operator( )( const std::string& p_value = EMPTY_STR ) const;
                    operator kv_store( ) const;
        kv_store    operator|( const tikz_option& p_other ) const;
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

        inline kv_store operator|( const kv_store& p_other ) const {
            kv_store res{ };
            res.m_options.append_range( m_options );
            res.m_options.append_range( p_other.m_options );
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
    };

    namespace OPT {
        const tikz_option TEXT_COLOR{ "text", COLOR_TEXT.to_string( ) };
        const tikz_option COLOR{ "color", COLOR_TEXT.to_string( ) };
        const tikz_option DRAW{ "draw", COLOR_TEXT.to_string( ) };
        const tikz_option FILL{ "fill", COLOR_TEXT.to_string( ) };

        const tikz_option DOUBLE{ "double", COLOR_WHITE.to_string( ) };
        const tikz_option DOUBLE_DISTANCE{ "double distance", "1.25pt" };

        const tikz_option ROTATE{ "rotate", "0" };
        const tikz_option XSCALE{ "xscale", "1" };
        const tikz_option YSCALE{ "yscale", "1" };

        const tikz_option OUTER_SEP{ "outer sep", "auto" };
        const tikz_option INNER_SEP{ "inner sep", ".3333em" };
        const tikz_option MINIMUM_SIZE{ "minimum size", ".3333em" };

        const tikz_option ROUNDED_CORNERS{ "rounded corners", "4pt" };
        const tikz_option SHARP_CORNERS{ "sharp corners" };

        const tikz_option LINE_WIDTH{ "line width", "1.25pt" };
        const tikz_option LW_LINE{ "line width", "1.25pt" };
        const tikz_option LW_VERY_THIN_OUTLINE{ "line width", ".5pt" };
        const tikz_option LW_THIN_OUTLINE{ "line width", ".75pt" };
        const tikz_option LW_OUTLINE{ "line width", "1pt" };
        const tikz_option LW_SUPPORT_LINE{ "line width", ".5pt" };
        const tikz_option LW_THICK_SUPPORT_LINE{ "line width", ".75pt" };

        const tikz_option LINE_CAP{ "line cap", "butt" };
        const tikz_option LINE_CAP_RECT{ "line cap", "rect" };
        const tikz_option LINE_CAP_BUTT{ "line cap", "butt" };
        const tikz_option LINE_CAP_ROUND{ "line cap", "round" };

        const tikz_option LINE_JOIN{ "line join", "round" };
        const tikz_option LINE_JOIN_MITER{ "line join", "miter" };
        const tikz_option LINE_JOIN_ROUND{ "line join", "round" };
        const tikz_option LINE_JOIN_BEVEL{ "line join", "bevel" };

        const tikz_option ARR_TIP_LATEX{ "-latex" };

        const tikz_option ANCHOR{ "anchor", "center" };
        const tikz_option ANCHOR_WEST{ "anchor", "west" };
        const tikz_option ANCHOR_EAST{ "anchor", "east" };
        const tikz_option ANCHOR_NORTH{ "anchor", "north" };
        const tikz_option ANCHOR_SOUTH{ "anchor", "south" };

        const tikz_option CIRCLE{ "circle" };
        const tikz_option CROSS_OUT{ "cross out", EMPTY_STR, EMPTY_STR, { "shapes.misc" } };
    } // namespace OPT

} // namespace TIKZ
