#pragma once
#include <map>
#include <string>

#include "defines.h"
#include "fragmentco.h"
#include "tikz_color.h"
#include "tikz_util.h"

namespace TIKZ {
    inline std::string pos_to_string( u64 p_pos ) {
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
        u64         m_wildcardId     = 0;
        bool        m_showSymbol     = false;
        bool        m_showId         = false;

        inline bool is_wildcard( ) const {
            return m_wildcardSymbol != EMPTY_STR;
        }
    };
    typedef std::map<u64, character_annotation> string_annotation;

    typedef u64 displ_t;
    struct str_displ_t {
        static constexpr displ_t IS_ROTATABLE = ( 1 << 0 );    // text should be rotated for
                                                               // vertical strings
        static constexpr displ_t SHOW_POSITIONS  = ( 1 << 1 ); // show individual position (S[ 1 ])
        static constexpr displ_t SHOW_CHARACTERS = ( 1 << 2 ); // show character annotations
        static constexpr displ_t SHOW_WILDCARDS  = ( 1 << 3 ); // show wildcard annotations
        static constexpr displ_t GROUP_POSITIONS = ( 1 << 4 ); // group consecutive positions
                                                               // (S[ 1 ], S[ 0 .. 3 ]
        static constexpr displ_t USE_TYPEWRITER = ( 1 << 5 );  // use typewriter font to
                                                               // display the name
                                                               // (instead of math mode)

        static constexpr displ_t MAX = ( 1 << 6 );

        // helper aliases
        static constexpr displ_t CHARACTERS       = SHOW_POSITIONS | SHOW_CHARACTERS;
        static constexpr displ_t FRAGMENT         = SHOW_POSITIONS | GROUP_POSITIONS | IS_ROTATABLE;
        static constexpr displ_t NAME             = GROUP_POSITIONS;
        static constexpr displ_t NAME_ROTATE      = IS_ROTATABLE | GROUP_POSITIONS;
        static constexpr displ_t POSITION         = SHOW_POSITIONS;
        static constexpr displ_t SINGLE_CHARACTER = USE_TYPEWRITER;
        static constexpr displ_t FRAGMENT_WILDCARD
            = SHOW_POSITIONS | GROUP_POSITIONS | IS_ROTATABLE | SHOW_WILDCARDS;
        static constexpr displ_t POSITION_WILDCARD = SHOW_POSITIONS | SHOW_WILDCARDS;
    };

    enum align_t { AN_CENTER = 0, AN_BEGIN = 1, AN_END = 2 };

    struct stylized_string {
        enum class fragment_t { FT_FULL, FT_FRAGMENT, FT_CHARACTER, FT_WILDCARD };

        displ_t     m_displayStyle = str_displ_t::FRAGMENT;
        fragmentco  m_fragment;
        align_t     m_labelAlign = AN_CENTER;
        color       m_color      = COLOR_TEXT;
        color       m_fillColor  = COLOR_TEXT.to_flavor_bg( );
        std::string m_name = EMPTY_STR; // stores name of the string, actual characters are stored
                                        // as annotations
        string_annotation m_annotation = { };

        // constructors for when individual characters should be printed
        stylized_string( const std::string& p_data, const std::string& p_name = EMPTY_STR,
                         displ_t p_displayStyle = str_displ_t::CHARACTERS,
                         align_t p_labelAlign = AN_CENTER, color p_color = COLOR_TEXT,
                         color p_fillColor = COLOR_TEXT.to_flavor_bg( ) )
            : m_displayStyle{ p_displayStyle }, m_fragment{ 0, p_data.length( ) },
              m_labelAlign{ p_labelAlign }, m_color{ p_color }, m_fillColor{ p_fillColor },
              m_name{ p_name } {
            for( u64 i = 0; i < p_data.length( ); ++i ) {
                m_annotation[ i ].m_symbol = std::string{ p_data[ i ] };
            }
        }

        stylized_string( const std::deque<std::string>& p_data,
                         const std::string&             p_name = EMPTY_STR,
                         displ_t p_displayStyle                = ( str_displ_t::CHARACTERS ),
                         align_t p_labelAlign = AN_CENTER, color p_color = COLOR_TEXT,
                         color p_fillColor = COLOR_TEXT.to_flavor_bg( ) )
            : m_displayStyle{ p_displayStyle }, m_fragment{ 0, p_data.size( ) },
              m_labelAlign{ p_labelAlign }, m_color{ p_color }, m_fillColor{ p_fillColor },
              m_name{ p_name } {
            for( u64 i = 0; i < p_data.size( ); ++i ) { m_annotation[ i ].m_symbol = p_data[ i ]; }
        }

        // constructors
        stylized_string( const std::string& p_name, fragmentco p_fragment,
                         displ_t p_displayStyle = str_displ_t::FRAGMENT,
                         align_t p_labelAlign = AN_CENTER, color p_color = COLOR_TEXT,
                         color             p_fillColor  = COLOR_TEXT.to_flavor_bg( ),
                         string_annotation p_annotation = { } )
            : m_displayStyle{ ( p_displayStyle ) }, m_fragment{ p_fragment },
              m_labelAlign{ p_labelAlign }, m_color{ p_color }, m_fillColor{ p_fillColor },
              m_name{ p_name }, m_annotation{ p_annotation } {
        }

        inline std::string operator[]( u64 p_pos ) const {
            return character( p_pos );
        }

        inline std::string& operator[]( u64 p_pos ) {
            return annotation_at_pos( p_pos ).m_symbol;
        }

        inline bool has_annotation_at_pos( u64 p_pos ) const {
            return !!m_annotation.count( p_pos );
        }

        inline character_annotation annotation_at_pos( u64 p_pos ) const {
            if( m_annotation.count( p_pos ) ) { return m_annotation.at( p_pos ); }
            return { };
        }

        inline character_annotation& annotation_at_pos( u64 p_pos ) {
            return m_annotation[ p_pos ];
        }

        inline bool has_character( u64 p_pos ) const {
            return annotation_at_pos( p_pos ).m_symbol != EMPTY_STR;
        }

        inline std::string character( u64 p_pos ) const {
            return annotation_at_pos( p_pos ).m_symbol;
        }

        inline bool has_wildcard( u64 p_pos ) const {
            return annotation_at_pos( p_pos ).is_wildcard( );
        }

        inline void highlight_position( u64 p_pos, color p_color ) {
            m_annotation[ p_pos ].m_textColor = p_color;
            m_annotation[ p_pos ].m_bgColor   = p_color.to_bg( );
        }

        inline void highlight_position( u64 p_pos, color p_color, color p_bgColor ) {
            m_annotation[ p_pos ].m_textColor = p_color;
            m_annotation[ p_pos ].m_bgColor   = p_bgColor;
        }

        inline u64 length( ) const {
            return m_fragment.length( );
        }

        inline bool rotatable( ) const {
            return m_displayStyle & str_displ_t::IS_ROTATABLE;
        }

        inline std::pair<fragment_t, fragmentco> next( u64 p_closedBegin ) const {
            if( p_closedBegin < m_fragment.closed_begin( ) ) {
                p_closedBegin = m_fragment.closed_begin( );
            }

            u64  end = p_closedBegin;
            auto tp  = fragment_t::FT_FRAGMENT;
            if( !( m_displayStyle & str_displ_t::SHOW_POSITIONS ) ) { tp = fragment_t::FT_FULL; }

            while( end < m_fragment.open_end( ) ) {
                if( ( m_displayStyle & str_displ_t::SHOW_WILDCARDS ) && has_wildcard( end ) ) {
                    if( end == p_closedBegin ) {
                        return { fragment_t::FT_WILDCARD,
                                 fragmentco{ p_closedBegin, p_closedBegin + 1 } };
                    }
                    break;
                }
                if( ( m_displayStyle & str_displ_t::SHOW_CHARACTERS ) && has_character( end ) ) {
                    if( end == p_closedBegin ) {
                        return { fragment_t::FT_CHARACTER,
                                 fragmentco{ p_closedBegin, p_closedBegin + 1 } };
                    }
                    break;
                }
                if( !( m_displayStyle & str_displ_t::GROUP_POSITIONS ) ) {
                    return { tp, fragmentco{ p_closedBegin, p_closedBegin + 1 } };
                }
                ++end;
            }
            return { tp, fragmentco{ p_closedBegin, end } };
        }

        inline std::pair<std::string, std::string>
        render_fragment( std::pair<fragment_t, fragmentco> p_fragment ) const {
            switch( p_fragment.first ) {
            case fragment_t::FT_CHARACTER: {
                return { character( p_fragment.second.closed_begin( ) ),
                         text_typewriter( VSIZE_CORRECTION
                                          + character( p_fragment.second.closed_begin( ) ) ) };
            }
            case fragment_t::FT_FRAGMENT: {
                if( p_fragment.second.length( ) == 1 ) {
                    return {
                        math_mode( m_name + pos_to_string( p_fragment.second.closed_begin( ) ) ),
                        textsize_footnotesize(
                            math_mode( VSIZE_CORRECTION + m_name
                                       + pos_to_string( p_fragment.second.closed_begin( ) ) ) ) };
                } else {
                    return {
                        math_mode( m_name + frag_to_string( p_fragment.second ) ),
                        textsize_footnotesize( math_mode(
                            VSIZE_CORRECTION + m_name + frag_to_string( p_fragment.second ) ) ) };
                }
            }
            case fragment_t::FT_FULL: {
                if( m_displayStyle & str_displ_t::USE_TYPEWRITER ) {
                    return { m_name, text_typewriter( VSIZE_CORRECTION + m_name ) };
                } else {
                    return {
                        math_mode( m_name ),
                        m_name.length( ) < 3
                            ? textsize_small( math_mode( VSIZE_CORRECTION + m_name ) )
                            : textsize_footnotesize( math_mode( VSIZE_CORRECTION + m_name ) ) };
                }
            }
            default:
            case fragment_t::FT_WILDCARD: {
                return { EMPTY_STR, EMPTY_STR };
            }
            }
        }

        inline stylized_string add_wildcards( const std::deque<u64>& p_wildcardPositions,
                                              bool p_showId = false, bool p_showSymbol = false,
                                              u64                p_startId  = 0,
                                              const std::string& p_wildcard = WILDCARD ) const {
            auto res = *this;
            for( u64 i : p_wildcardPositions ) {
                res.m_annotation[ i ].m_wildcardSymbol = p_wildcard;
                res.m_annotation[ i ].m_wildcardId     = p_startId++;
                res.m_annotation[ i ].m_showSymbol     = p_showSymbol;
                res.m_annotation[ i ].m_showId         = p_showId;
            }
            return res;
        }

        inline stylized_string add_wildcards( const std::string& p_mask, bool p_showId = false,
                                              bool p_showSymbol = false, u64 p_startId = 0,
                                              const std::string& p_wildcard = WILDCARD ) const {
            std::deque<u64> pos{ };
            for( size_t i = 0; i < p_mask.size( ); ++i ) {
                if( std::string{ p_mask[ i ] } == p_wildcard ) { pos.push_back( i ); }
            }
            return add_wildcards( pos, p_showId, p_showSymbol, p_startId, p_wildcard );
        }

        inline stylized_string color_invert( ) const {
            auto res = *this;
            std::swap( res.m_color, res.m_fillColor );
            return res;
        }

        inline stylized_string slice( fragmentco p_fragment ) const {
            auto res         = *this;
            res.m_fragment   = p_fragment;
            res.m_annotation = { };

            for( u64 i = p_fragment.closed_begin( ); i < p_fragment.open_end( ); ++i ) {
                if( has_annotation_at_pos( i ) ) { res.m_annotation[ i ] = annotation_at_pos( i ); }
            }
            return res;
        }

        inline stylized_string replace_data( const std::string& p_data,
                                             u64                p_closedBegin = 0 ) const {
            auto res = *this;
            for( u64 i = p_closedBegin, j = 0; i < m_fragment.open_end( ) && j < p_data.size( );
                 ++i, ++j ) {
                res.m_annotation[ i ].m_symbol = std::string{ p_data[ j ] };
            }
            return res;
        }

        inline stylized_string replace_data( displ_t            p_displayStyleOverride,
                                             const std::string& p_data,
                                             u64                p_closedBegin = 0 ) const {
            auto res           = replace_data( p_data, p_closedBegin );
            res.m_displayStyle = p_displayStyleOverride;
            return res;
        }

        inline stylized_string replace_data( const std::deque<std::string>& p_data,
                                             u64 p_closedBegin = 0 ) const {
            auto res = *this;
            for( u64 i = p_closedBegin, j = 0; i < m_fragment.open_end( ) && j < p_data.size( );
                 ++i, ++j ) {
                res.m_annotation[ i ].m_symbol = p_data[ j ];
            }
            return res;
        }

        inline stylized_string replace_data( displ_t                        p_displayStyleOverride,
                                             const std::deque<std::string>& p_data,
                                             u64 p_closedBegin = 0 ) const {
            auto res           = replace_data( p_data, p_closedBegin );
            res.m_displayStyle = p_displayStyleOverride;
            return res;
        }
    };
} // namespace TIKZ
