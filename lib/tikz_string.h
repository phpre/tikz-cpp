#pragma once

#include <cstdio>
#include <map>
#include <string>

#include "ag_slice.h"
#include "breakpoint.h"
#include "tikz.h"
#include "tikz_graph.h"

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
    color m_bgColor   = COLOR_NONE;
    color m_textColor = COLOR_NONE;
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

enum str_displ_t {
    SDT_FRAGMENT = 0,     // print name, combine consecutive positions
    SDT_NAME,             // only ever print name of string
    SDT_NAME_ROTATE,      // only ever print name of string
    SDT_POSITION,         // name of a string, don't group positions
    SDT_CHARACTERS,       // actual string
    SDT_SINGLE_CHARACTER, // string is just a single character
};

enum align_t { AN_CENTER = 0, AN_BEGIN = 1, AN_END = 2 };

struct stylized_string {
    std::string       m_data; // name or actual string depending on m_displayStyle
    str_displ_t       m_displayStyle;
    fragmentco        m_fragment;
    color             m_color      = COLOR_TEXT;
    string_annotation m_annotation = { };
    align_t           m_labelAlign = AN_CENTER;

    inline character_annotation annotation_at_pos( u32 p_pos ) const {
        if( m_annotation.count( p_pos ) ) { return m_annotation.at( p_pos ); }
        return { };
    }

    inline void highlight_position( u32 p_pos, color p_color ) {
        m_annotation[ p_pos ].m_textColor = p_color;
        m_annotation[ p_pos ].m_bgColor   = p_color.to_bg( );
    }

    inline u32 length( ) const {
        return m_fragment.length( );
    }

    inline bool rotatable( ) const {
        return m_displayStyle == SDT_FRAGMENT || m_displayStyle == SDT_NAME_ROTATE;
    }

    inline bool print_single_characters( ) const {
        return m_displayStyle == SDT_POSITION || m_displayStyle == SDT_CHARACTERS;
    }

    inline std::string render_pos( u32 p_pos ) const {
        switch( m_displayStyle ) {
        case SDT_POSITION: return math_mode( m_data + pos_to_string( p_pos ) );
        case SDT_CHARACTERS: return ( *this )[ p_pos ];
        default: return EMPTY_STR;
        }
    }

    inline std::string render_complete( ) const {
        switch( m_displayStyle ) {
        case SDT_FRAGMENT: return math_mode( m_data + frag_to_string( m_fragment ) );
        case SDT_NAME_ROTATE:
        case SDT_NAME: return math_mode( m_data );
        case SDT_SINGLE_CHARACTER: return m_data;
        default: return EMPTY_STR;
        }
    }

    inline std::string resize_rendering( const std::string& p_render ) const {
        auto res = vsize_correction( p_render );
        switch( m_displayStyle ) {
        case SDT_POSITION:
        case SDT_FRAGMENT: return textsize_footnotesize( res );
        case SDT_NAME_ROTATE:
        case SDT_NAME:
            if( m_data.length( ) < 3 ) {
                return textsize_small( res );
            } else {
                return textsize_footnotesize( res );
            }
        case SDT_CHARACTERS:
        case SDT_SINGLE_CHARACTER: return text_typewriter( textsize_small( res ) );
        default: return res;
        }
    }

    inline std::string operator[]( u32 p_pos ) const {
        if( m_displayStyle == SDT_SINGLE_CHARACTER ) { return m_data; }
        if( m_displayStyle == SDT_CHARACTERS && p_pos < m_data.length( ) ) {
            return std::string{ m_data[ p_pos ] };
        }
        return EMPTY_STR;
    }

    inline stylized_string slice( fragmentco p_fragment ) const {
        return stylized_string{ m_data, p_fragment, m_displayStyle, m_color, m_annotation };
    }

    inline stylized_string slice( fragmentco p_fragment, const std::string& p_dataOverride ) const {
        return stylized_string{ p_dataOverride, p_fragment, SDT_CHARACTERS, m_color, m_annotation };
    }

    inline stylized_string slice( fragmentco p_fragment, char p_dataOverride ) const {
        return stylized_string{ std::string{ p_dataOverride }, p_fragment, SDT_SINGLE_CHARACTER,
                                m_color, m_annotation };
    }

    stylized_string( const std::string& p_string, str_displ_t p_displayStyle = SDT_FRAGMENT,
                     color p_color = COLOR_TEXT, const string_annotation& p_annotation = { } )
        : m_data{ p_string }, m_displayStyle{ p_displayStyle },
          m_fragment{ 0, static_cast<u32>( p_string.length( ) ) }, m_color{ p_color },
          m_annotation{ p_annotation } {
    }

    stylized_string( char p_char, color p_color = COLOR_TEXT,
                     const string_annotation& p_annotation = { } )
        : m_data{ p_char }, m_displayStyle{ str_displ_t::SDT_SINGLE_CHARACTER }, m_fragment{ 0, 1 },
          m_color{ p_color }, m_annotation{ p_annotation } {
    }

    stylized_string( const std::string& p_string, fragmentco p_fragment,
                     str_displ_t p_displayStyle = SDT_FRAGMENT, color p_color = COLOR_TEXT,
                     const string_annotation& p_annotation = { } )
        : m_data{ p_string }, m_displayStyle{ p_displayStyle }, m_fragment{ p_fragment },
          m_color{ p_color }, m_annotation{ p_annotation } {
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
