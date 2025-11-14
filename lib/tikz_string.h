#pragma once

#include <cstdio>
#include <map>
#include <string>

#include "ag_slice.h"
#include "breakpoint.h"
#include "tikz.h"
#include "tikz_graph.h"
#include "wildcard.h"

constexpr double CHAR_WIDTH  = .5;
constexpr double CHAR_HEIGHT = .5;
constexpr double CHAR_GLOW   = .075;

constexpr color MAT_COL = COLOR_C1;
constexpr color DEL_COL = COLOR_C3;
constexpr color INS_COL = COLOR_C4;
constexpr color SUB_COL = COLOR_C5;
constexpr color SEP_COL = COLOR_C2;

inline std::string pos_to_string( u32 p_pos ) {
    std::string res;
    res += "\\position{" + std::to_string( p_pos ) + "}";
    return res;
}

inline std::string frag_to_string( fragmentco p_frag ) {
    if( p_frag.length( ) == 0 ) { return EMPTY_STR; }
    if( p_frag.length( ) == 1 ) { return pos_to_string( p_frag.closed_begin( ) ); }
    std::string res;
    res += "\\fragment{" + std::to_string( p_frag.closed_begin( ) ) + "}{";
    res += std::to_string( p_frag.closed_end( ) ) + "}";
    return res;
}

struct character_annotation {
    color       m_bgColor        = COLOR_NONE;
    color       m_textColor      = COLOR_NONE;
    std::string m_symbol         = EMPTY_STR;
    std::string m_wildcardSymbol = EMPTY_STR;
    u32         m_wildcardId     = 0;
    bool        m_showSymbol     = false;
    bool        m_showId         = false;

    inline bool is_wildcard( ) const {
        return m_wildcardSymbol != EMPTY_STR;
    }
};
typedef std::map<u32, character_annotation> string_annotation;

// display modes
// m_string = "P"
// - P[ 0 ] or P[ 0 .. 5 ] (default)
// - P
// - P[ 0 ] P[ 1 ] P [ 2 ]
//
// m_string = "abcdefg"
// - a b c d e

typedef u32          displ_t;
static constexpr u32 SDT_IS_ROTATABLE        = ( 1 << 0 );
static constexpr u32 SDT_SHOW_POSITIONS      = ( 1 << 1 );
static constexpr u32 SDT_SHOW_CHARACTERS     = ( 1 << 2 );
static constexpr u32 SDT_SHOW_WILDCARDS      = ( 1 << 3 );
static constexpr u32 SDT_GROUP_POSITIONS     = ( 1 << 4 );
static constexpr u32 SDT_IS_SINGLE_CHARACTER = ( 1 << 5 );

enum str_displ_t {
    SDT_CHARACTERS = 0,    // actual string
    SDT_FRAGMENT,          // print name, combine consecutive positions
    SDT_NAME,              // only ever print name of string (never rotate name)
    SDT_NAME_ROTATE,       // only ever print name of string (rotate name if string is
                           // printed vertically)
    SDT_POSITION,          // name of a string, don't group positions
    SDT_SINGLE_CHARACTER,  // string is just a single character
    SDT_FRAGMENT_WILDCARD, // print name, combine consecutive positions, highlight
                           // wildcards
    SDT_POSITION_WILDCARD, // name of a string, don't group positions, highlight
                           // wildcards
};

constexpr displ_t from_str_displ_t( str_displ_t p_type ) {
    switch( p_type ) {
    case SDT_FRAGMENT: return SDT_SHOW_POSITIONS | SDT_GROUP_POSITIONS | SDT_IS_ROTATABLE;
    case SDT_NAME_ROTATE: return SDT_IS_ROTATABLE;
    case SDT_POSITION: return SDT_SHOW_POSITIONS;
    case SDT_CHARACTERS: return SDT_SHOW_CHARACTERS;
    case SDT_SINGLE_CHARACTER: return SDT_IS_SINGLE_CHARACTER;
    case SDT_FRAGMENT_WILDCARD:
        return SDT_SHOW_POSITIONS | SDT_GROUP_POSITIONS | SDT_IS_ROTATABLE | SDT_SHOW_WILDCARDS;
    case SDT_POSITION_WILDCARD: return SDT_SHOW_POSITIONS | SDT_SHOW_WILDCARDS;
    default:
    case SDT_NAME: return 0;
    }
}

enum align_t { AN_CENTER = 0, AN_BEGIN = 1, AN_END = 2 };

struct stylized_string {
    enum class fragment_t { FT_FULL, FT_FRAGMENT, FT_CHARACTER, FT_WILDCARD };

    displ_t     m_displayStyle = from_str_displ_t( SDT_FRAGMENT );
    fragmentco  m_fragment;
    color       m_color      = COLOR_TEXT;
    align_t     m_labelAlign = AN_CENTER;
    color       m_fillColor  = COLOR_TEXT.to_flavor_bg( );
    std::string m_data; // stores name of the string, actual characters are stored
                        // as annotations
    string_annotation m_annotation = { };

    stylized_string( const std::string& p_string, str_displ_t p_displayStyle = SDT_FRAGMENT,
                     color p_color = COLOR_TEXT, color p_fillColor = COLOR_TEXT.to_flavor_bg( ),
                     const string_annotation& p_annotation = { } )
        : m_data{ p_string }, m_displayStyle{ p_displayStyle },
          m_fragment{ 0, static_cast<u32>( p_string.length( ) ) }, m_color{ p_color },
          m_fillColor{ p_fillColor }, m_annotation{ p_annotation } {
    }

    stylized_string( char p_char, color p_color = COLOR_TEXT,
                     color                    p_fillColor  = COLOR_TEXT.to_flavor_bg( ),
                     const string_annotation& p_annotation = { } )
        : m_data{ p_char }, m_displayStyle{ str_displ_t::SDT_SINGLE_CHARACTER }, m_fragment{ 0, 1 },
          m_color{ p_color }, m_fillColor{ p_fillColor }, m_annotation{ p_annotation } {
    }

    stylized_string( const std::string& p_string, fragmentco p_fragment,
                     str_displ_t p_displayStyle = SDT_FRAGMENT, color p_color = COLOR_TEXT,
                     color                    p_fillColor  = COLOR_TEXT.to_flavor_bg( ),
                     const string_annotation& p_annotation = { } )
        : m_data{ p_string }, m_displayStyle{ p_displayStyle }, m_fragment{ p_fragment },
          m_color{ p_color }, m_fillColor{ p_fillColor }, m_annotation{ p_annotation } {
    }

    inline std::string operator[]( u32 p_pos ) const {
        return character( p_pos );
    }

    inline std::string& operator[]( u32 p_pos ) {
        return annotation_at_pos( p_pos ).m_symbol;
    }

    inline character_annotation annotation_at_pos( u32 p_pos ) const {
        if( m_annotation.count( p_pos ) ) { return m_annotation.at( p_pos ); }
        return { };
    }

    inline character_annotation& annotation_at_pos( u32 p_pos ) {
        return m_annotation[ p_pos ];
    }

    inline bool has_character( u32 p_pos ) const {
        return annotation_at_pos( p_pos ).m_symbol != EMPTY_STR;
    }

    inline std::string character( u32 p_pos ) const {
        return annotation_at_pos( p_pos ).m_symbol;
    }

    inline bool has_wildcard( u32 p_pos ) const {
        return annotation_at_pos( p_pos ).is_wildcard( );
    }

    inline void highlight_position( u32 p_pos, color p_color ) {
        m_annotation[ p_pos ].m_textColor = p_color;
        m_annotation[ p_pos ].m_bgColor   = p_color.to_bg( );
    }

    inline void highlight_position( u32 p_pos, color p_color, color p_bgColor ) {
        m_annotation[ p_pos ].m_textColor = p_color;
        m_annotation[ p_pos ].m_bgColor   = p_bgColor;
    }

    inline u32 length( ) const {
        return m_fragment.length( );
    }

    inline bool rotatable( ) const {
        return m_displayStyle & SDT_IS_ROTATABLE;
    }

    inline std::pair<fragment_t, fragmentco> next( u32 p_closedBegin
                                                   = m_fragment.closed_begin( ) ) {
        if( !( m_displayStyle & SDT_SHOW_POSITIONS ) ) {
            return { FT_FULL, fragmentco{ p_closedBegin, m_fragment.open_end( ) } };
        }

        u32  end = p_closedBegin;
        auto tp  = FT_FRAGMENT;
        while( end < m_fragment.closed_end( ) ) {
            if( ( m_displayStyle & SDT_SHOW_CHARACTERS ) && has_character( end ) ) {
                if( end == p_closedBegin ) {
                    tp = FT_CHARACTER;
                } else {
                    --end;
                }
                break;
            }
            if( ( m_displayStyle & SDT_SHOW_WILDCARDS ) && has_wildcard( end ) ) {
                if( end == p_closedBegin ) {
                    tp = FT_WILDCARD;
                } else {
                    --end;
                }
                break;
            }
            if( !( m_displayStyle & SDT_GROUP_POSITIONS ) ) { break; }
            ++end;
        }
        return { tp, fragmentco{ p_closedBegin, end } };
    }

    inline std::pair<std::string, std::string>
    render_fragment( std::pair<fragment_t, fragmentco> p_fragment ) {
        switch( p_fragment.first ) {
        case FT_CHARACTER: {
            return { character( p_fragment.second.closed_begin( ) ),
                     text_typewriter( character( p_fragment.second.closed_begin( ) ) ) };
        }
        case FT_FRAGMENT: {
            if( p_fragment.second.length( ) == 1 ) {
                return { math_mode( m_data + pos_to_string( p_fragment.second.closed_begin( ) ) ),
                         textsize_footnotesize( math_mode(
                             m_data + pos_to_string( p_fragment.second.closed_begin( ) ) ) ) };
            } else {
                return { math_mode( m_data + frag_to_string( p_fragment.second ) ),
                         textsize_footnotesize(
                             math_mode( m_data + frag_to_string( p_fragment.second ) ) ) };
            }
            return EMPTY_STR;
        }
        case FT_FULL: {
            if( m_displayStyle & SDT_IS_SINGLE_CHARACTER ) {
                return { m_data, text_typewriter( m_data ) };
            } else {
                return { math_mode( m_data ), m_data.length( ) < 3
                                                  ? textsize_small( math_mode( m_data ) )
                                                  : textsize_footnotesize( math_mode( m_data ) ) };
            }
        }
        default:
        case FT_WILDCARD: {
            return { EMPTY_STR, EMPTY_STR };
        }
        }
    }

    inline stylized_string add_wildcards( const std::deque<u32>& p_wildcardPositions,
                                          bool p_showId = false, bool p_showSymbol = false,
                                          u32                p_startId  = 0,
                                          const std::string& p_wildcard = WILDCARD ) const {
        auto res = *this;
        for( u32 i : p_wildcardPositions ) {
            res.m_annotation[ i ].m_wildcardSymbol = p_wildcard;
            res.m_annotation[ i ].m_wildcardId     = p_startId++;
            res.m_annotation[ i ].m_showSymbol     = p_showSymbol;
            res.m_annotation[ i ].m_showId         = p_showId;
        }
        return res;
    }

    inline stylized_string add_wildcards( const std::string& p_mask, bool p_showId = false,
                                          bool p_showSymbol = false, u32 p_startId = 0,
                                          const std::string& p_wildcard = WILDCARD ) const {
        std::deque<u32> pos{ };
        for( size_t i = 0; i < p_mask.size( ); ++i ) {
            if( std::string{ p_mask[ i ] } == p_wildcard ) { pos.push_back( i ); }
        }
        return add_wildcards( res, p_showId, p_showSymbol, p_startId, p_wildcard );
    }

    inline stylized_string color_invert( ) const {
        return stylized_string{ m_data,      m_fragment, m_displayStyle,
                                m_fillColor, m_color,    m_annotation };
    }

    inline stylized_string slice( fragmentco p_fragment ) const {
        return stylized_string{ m_data,  p_fragment,  m_displayStyle,
                                m_color, m_fillColor, m_annotation };
    }

    inline stylized_string slice( fragmentco p_fragment, const std::string& p_dataOverride ) const {
        return stylized_string{ p_dataOverride, p_fragment,  SDT_CHARACTERS,
                                m_color,        m_fillColor, m_annotation };
    }

    inline stylized_string slice( fragmentco p_fragment, char p_dataOverride ) const {
        return stylized_string{ std::string{ p_dataOverride },
                                p_fragment,
                                SDT_SINGLE_CHARACTER,
                                m_color,
                                m_fillColor,
                                m_annotation };
    }
};

color color_for_bp( const breakpoint& p_bp );

void print_separator( FILE* p_out, tikz_point p_PtopLeft, tikz_point p_TtopLeft, color p_color,
                      u32 p_startIndent = 1, u32 p_indent = 4 );

// Prints a string S
void print_string( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                   u32 p_startIndent = 1, u32 p_indent = 4 );

void print_string_vertical( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                            u32 p_startIndent = 1, u32 p_indent = 4 );

// assumes T is on top, P on bottom
void print_matched_string_pair( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft,
                                const stylized_string& p_T, tikz_point p_TtopLeft, color p_bgColor,
                                u32 p_startIndent = 1, u32 p_indent = 4 );

// Prints alignment P to T
std::pair<tikz_point, tikz_point>
print_alignment( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft,
                 const stylized_string& p_T, tikz_point p_TtopLeft, const breakpoint_repn& p_brpnt,
                 bool p_printBreakpoints = true, u32 p_startIndent = 1, u32 p_indent = 4 );

void print_alignment_graph_label( FILE* p_out, const stylized_string& p_Pname,
                                  const stylized_string& p_Tname,
                                  tikz_point             p_gridTopLeft = tikz_point{ 0.0, 0.0 },
                                  u32 p_startIndent = 1, u32 p_indent = 4 );

vertex_grid print_alignment_graph_simple( FILE* p_out, fragmentco p_fragP, fragmentco p_fragT,
                                          tikz_point p_gridCellSize
                                          = tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                                          tikz_point p_gridTopLeft = tikz_point{ 0.0, 0.0 },
                                          color      p_color       = COLOR_BLACK.deemphasize( ),
                                          u32 p_startIndent = 1, u32 p_indent = 4 );

vertex_grid print_alignment_graph( FILE* p_out, const std::string& p_P, fragmentco p_fragP,
                                   const std::string& p_T, fragmentco p_fragT,
                                   tikz_point p_gridCellSize
                                   = tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                                   tikz_point p_gridTopLeft = tikz_point{ 0.0, 0.0 },
                                   color      p_vertexColor = COLOR_BLACK.deemphasize( ),
                                   u32 p_startIndent = 1, u32 p_indent = 4 );

void print_alignment_on_coordinates( FILE* p_out, const vertex_grid& p_vg,
                                     const breakpoint_repn& p_brpnt, u32 p_startIndent = 1,
                                     u32 p_indent = 4 );

void print_graph_slice_on_coordinates( FILE* p_out, const vertex_grid& p_vg,
                                       const graph_slice& p_gs, color p_color,
                                       color p_portalColor      = COLOR_NONE,
                                       color p_innerPortalColor = COLOR_NONE,
                                       color p_topColor         = COLOR_NONE,
                                       color p_bottomColor = COLOR_NONE, u32 p_startIndent = 1,
                                       u32 p_indent = 4 );

constexpr double DEFAULT_SLICE_SPACING = 0.25;
void             print_graph_slices_stylized(
                FILE* p_out, const std::deque<graph_slice>& p_gs, breakpoint_repn p_alignment,
                color p_portalVertexColor = COLOR_BLACK.deemphasize( ),
                color p_innerPortalVertexColor = COLOR_NONE, color p_topVertexColor = SEP_COL.deemphasize( ),
                color p_bottomVertexColor = SEP_COL.deemphasize( ), color p_pureColor = MAT_COL.deemphasize( ),
                color  p_impureColor = SUB_COL.deemphasize( ),
                color  p_leftColor   = COLOR_TEXT.deemphasize_strong( ),
                color  p_rightColor  = COLOR_TEXT.deemphasize_strong( ),
                double p_sliceSpacing = DEFAULT_SLICE_SPACING, u32 p_startIndent = 1, u32 p_indent = 4 );

void print_graph_slices( FILE* p_out, const std::deque<graph_slice>& p_gs,
                         breakpoint_repn p_alignment, const std::string& p_P,
                         const std::string& p_T,
                         color              p_portalVertexColor = COLOR_BLACK.deemphasize( ),
                         color  p_internalPortalVertexColor     = COLOR_BLACK.deemphasize( ),
                         color  p_topVertexColor                = SEP_COL.deemphasize( ),
                         color  p_bottomVertexColor             = SEP_COL.deemphasize( ),
                         double p_sliceSpacing = DEFAULT_SLICE_SPACING, u32 p_startIndent = 1,
                         u32 p_indent = 4 );

void print_graph_slices_t_labels( FILE* p_out, const std::deque<graph_slice>& p_gs,
                                  const stylized_string& p_Tname, const stylized_string& p_Pname,
                                  bool   p_labelEqualParts = false,
                                  double p_labelDistance   = DEFAULT_SLICE_SPACING,
                                  double p_sliceSpacing    = DEFAULT_SLICE_SPACING,
                                  u32 p_startIndent = 1, u32 p_indent = 4 );

void print_graph_slices_p_labels( FILE* p_out, const std::deque<graph_slice>& p_gs, u32 p_d,
                                  bool   p_guideLines    = true,
                                  double p_labelDistance = DEFAULT_SLICE_SPACING,
                                  double p_sliceSpacing  = DEFAULT_SLICE_SPACING,
                                  u32 p_startIndent = 1, u32 p_indent = 4 );
