#pragma once
#include <deque>
#include <format>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz_color.h"

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
        kv_store    operator|( const kv_store& p_other ) const;
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

        const tikz_option FILL_OPACITY{ "fill opacity", "1" };

        const kv_store CLIP            = tikz_option{ "clip" };
        const kv_store TRANSFORM_SHAPE = tikz_option{ "transform shape" };

        const tikz_option DOUBLE{ "double", COLOR_WHITE.to_string( ) };
        const tikz_option DOUBLE_DISTANCE{ "double distance", "1.25pt" };

        const tikz_option SHAPE_BORDER_ROTATE{ "shape border rotate", "0" };
        const tikz_option ROTATE{ "rotate", "0" };
        const tikz_option XSCALE{ "xscale", "1" };
        const tikz_option YSCALE{ "yscale", "1" };
        const tikz_option SCALE{ "scale", "1" };

        const tikz_option XSHIFT{ "xshift", "0" };
        const tikz_option YSHIFT{ "yshift", "0" };

        const tikz_option OUTER_SEP{ "outer sep", "auto" };
        const tikz_option INNER_SEP{ "inner sep", ".3333em" };
        const tikz_option MINIMUM_SIZE{ "minimum size", ".3333em" };

        const tikz_option ROUNDED_CORNERS{ "rounded corners", "4pt" };
        const kv_store    SHARP_CORNERS = tikz_option{ "sharp corners" };

        const tikz_option LINE_WIDTH{ "line width", "1.25pt" };
        const kv_store    LW_DOUBLE_BG          = tikz_option{ "line width", "2.25pt" };
        const kv_store    LW_LINE               = tikz_option{ "line width", "1.25pt" };
        const kv_store    LW_THICK_LINE         = tikz_option{ "line width", "1.5pt" };
        const kv_store    LW_VERY_THICK_LINE    = tikz_option{ "line width", "1.75pt" };
        const kv_store    LW_VERY_THIN_OUTLINE  = tikz_option{ "line width", ".5pt" };
        const kv_store    LW_THIN_OUTLINE       = tikz_option{ "line width", ".75pt" };
        const kv_store    LW_OUTLINE            = tikz_option{ "line width", "1pt" };
        const kv_store    LW_SUPPORT_LINE       = tikz_option{ "line width", ".5pt" };
        const kv_store    LW_THICK_SUPPORT_LINE = tikz_option{ "line width", ".75pt" };
        const kv_store    LW_PATTERN_LINE       = tikz_option{ "line width", ".75pt" };
        const kv_store    LW_PATTERN_THIN_LINE  = tikz_option{ "line width", ".5pt" };

        const tikz_option LINE_CAP{ "line cap", "butt" };
        const kv_store    LINE_CAP_RECT  = tikz_option{ "line cap", "rect" };
        const kv_store    LINE_CAP_BUTT  = tikz_option{ "line cap", "butt" };
        const kv_store    LINE_CAP_ROUND = tikz_option{ "line cap", "round" };

        const tikz_option LINE_JOIN{ "line join", "round" };
        const kv_store    LINE_JOIN_MITER = tikz_option{ "line join", "miter" };
        const kv_store    LINE_JOIN_ROUND = tikz_option{ "line join", "round" };
        const kv_store    LINE_JOIN_BEVEL = tikz_option{ "line join", "bevel" };

        const tikz_option ARR_TIP{ "-{Stealth[length=1.75pt 2.5 0, round, inset=0pt, angle'=45]}",
                                   EMPTY_STR,
                                   EMPTY_STR,
                                   { "arrows.meta" } };
        const tikz_option ARR_TIP_LATEX = ARR_TIP;

        const tikz_option ANCHOR{ "anchor", "center" };
        const kv_store    ANCHOR_WEST  = tikz_option{ "anchor", "west" };
        const kv_store    ANCHOR_EAST  = tikz_option{ "anchor", "east" };
        const kv_store    ANCHOR_NORTH = tikz_option{ "anchor", "north" };
        const kv_store    ANCHOR_SOUTH = tikz_option{ "anchor", "south" };

        const kv_store CIRCLE    = tikz_option{ "circle" };
        const kv_store RECTANGLE = tikz_option{ "rectangle" };
        const kv_store CROSS_OUT
            = tikz_option{ "cross out", EMPTY_STR, EMPTY_STR, { "shapes.misc" } };
        const tikz_option REGULAR_POLYGON{
            "regular polygon, regular polygon sides", "5", EMPTY_STR, { "shapes.geometric" } };
        const kv_store REGULAR_TRIANGLE = tikz_option{
            "regular polygon, regular polygon sides", "3", "3", { "shapes.geometric" } };

        const tikz_option RADIUS{ "radius", "3pt" };

        const tikz_option PATTERN_COLOR{ "pattern color", COLOR_TEXT.to_string( ) };
        const tikz_option PATTERN_ANGLE{ "angle", "0" };
        const tikz_option PATTERN_DISTANCE{ "distance", ".5pt" };

        inline tikz_option postaction( kv_store p_options ) {
            return tikz_option{ "postaction", EMPTY_STR, "{" + p_options.to_string( ) + "}",
                                p_options.libraries( ), p_options.packages( ) };
        }

        inline kv_store double_arrow( kv_store p_innerOptions = OPT::DRAW( COLOR_FILL_WHITE ),
                                      double p_doubleDistance = 1.25, double p_outlineWidth = .5 ) {
            // via
            // https://tex.stackexchange.com/questions/17950/how-can-i-draw-the-outline-of-a-path-in-tikz
            double lwfactor = 2;
            return LINE_WIDTH( std::format( "{:5.3f}pt", 2 * p_outlineWidth + p_doubleDistance ) )
                   | tikz_option{ std::string{ "-{Stealth[length=" }
                                      + std::format( "{:5.3f}pt 1/2*(cosec(22.5)+sec(45)), ",
                                                     lwfactor * p_doubleDistance )
                                      + "round, inset=0pt, angle'=45]}",
                                  EMPTY_STR,
                                  EMPTY_STR,
                                  { "arrows.meta" } }
                   | tikz_option{ std::format( "shorten <={:5.3f}pt", -p_outlineWidth ) }
                   | tikz_option{ std::format( "shorten >=cosec(45)*{:5.3f}pt", -p_outlineWidth ) }
                   | postaction(
                       LINE_WIDTH( std::format( "{:5.3f}pt", p_doubleDistance ) )
                       | tikz_option{ std::format( "shorten >={:5.3}pt", p_outlineWidth / 2 ) }
                       | tikz_option{ std::format( "shorten <=0pt" ) } | p_innerOptions );
        }

        inline kv_store pattern( const std::string& p_patternName, kv_store p_options = { } ) {
            auto libs = p_options.libraries( );
            libs.insert( "patterns.meta" );
            return tikz_option{ "pattern", EMPTY_STR,
                                std::format( "{{{}[{}]}}", p_patternName, p_options.to_string( ) ),
                                libs, p_options.packages( ) };
        }

        const kv_store PATTERN_LINES_45
            = OPT::pattern( "Lines", OPT::PATTERN_ANGLE( "45" ) | OPT::PATTERN_DISTANCE( "2.75pt" )
                                         | OPT::LW_PATTERN_LINE );
        const kv_store PATTERN_LINES_90
            = OPT::pattern( "Lines", OPT::PATTERN_ANGLE( "90" ) | OPT::PATTERN_DISTANCE( "2.75pt" )
                                         | OPT::LW_PATTERN_LINE );
        const kv_store PATTERN_LINES_135
            = OPT::pattern( "Lines", OPT::PATTERN_ANGLE( "-45" ) | OPT::PATTERN_DISTANCE( "2.75pt" )
                                         | OPT::LW_PATTERN_LINE );
        const kv_store PATTERN_LINES_180
            = OPT::pattern( "Lines", OPT::PATTERN_ANGLE( "180" ) | OPT::PATTERN_DISTANCE( "2.75pt" )
                                         | OPT::LW_PATTERN_LINE );
        const kv_store PATTERN_HATCH_45
            = OPT::pattern( "Hatch", OPT::PATTERN_ANGLE( "45" ) | OPT::PATTERN_DISTANCE( "2.75pt" )
                                         | OPT::LW_PATTERN_THIN_LINE );
    } // namespace OPT
} // namespace TIKZ
