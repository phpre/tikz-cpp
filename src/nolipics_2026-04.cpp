#include <string>
#include <vector>

#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT = "../figs/";
std::string TEXDIR_DEFAULT = "../tex/";
// std::string FONT_FILENAME   = "font_default";
std::string FONT_FILENAME   = "font";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      globals
//
// ---------------------------------------------------------------------------------------------

struct inference_graph {
    using cc_t = std::pair<bool, std::vector<u64>>;

  private:
    std::vector<bool> _isRed;
    std::vector<u64>  _color;

    void color( u64 p_vtx = 0, u64 p_color = 0 ) {
        if( !p_color ) {
            _color = std::vector<u64>( m_P.size( ) + m_T.size( ) + 1, 0 );
            _isRed = std::vector<bool>{ };
            _isRed.push_back( false );

            for( u64 i = 0; i < m_P.size( ) + m_T.size( ) + 1; ++i ) {
                _isRed.push_back( false );
                if( !_color[ i ] ) { color( i, _isRed.size( ) - 1 ); }
            }
            return;
        }

        _color[ p_vtx ] = p_color;
        for( const auto& n : m_edges[ p_vtx ] ) {
            auto next = neighbor( p_vtx, n );
            if( !_color[ next ] ) {
                color( next, p_color );
                if( !n.m_charT || !n.m_charP || n.m_charT != n.m_charP ) { _isRed.back( ) = true; }
            }
        }
    }

  public:
    const std::string                    m_P, m_T;
    std::vector<std::vector<breakpoint>> m_edges;

    inline u64 vertex_P( u64 p_posP ) const {
        return p_posP;
    }
    inline u64 vertex_T( u64 p_posT ) const {
        return m_P.size( ) + p_posT;
    }
    inline u64 vertex_bot( ) const {
        return m_P.size( ) + m_T.size( );
    }

    inline bool is_red( u64 p_vtx ) const {
        return _isRed[ _color[ p_vtx ] ];
    }

    inline std::string mid_label( u64 p_start, const std::string& p_pname,
                                  const std::string& p_tname ) const {
        if( p_start < m_P.size( ) ) {
            return p_pname + "_mid";
        } else if( p_start < m_T.size( ) + m_P.size( ) ) {
            return p_tname + "_mid";
        }
        return "";
    }

    inline std::string label_for_vtx( u64 p_vtx, const std::string& p_pname,
                                      const std::string& p_tname ) const {
        if( p_vtx < m_P.size( ) ) {
            return p_pname + "_" + std::to_string( p_vtx );
        } else if( p_vtx < m_T.size( ) + m_P.size( ) ) {
            return p_tname + "_" + std::to_string( p_vtx - m_P.size( ) );
        } else {
            return "bot";
        }
    }

    inline u64 neighbor( u64 p_vtx, breakpoint p_bp ) const {
        if( vertex_P( p_bp.m_posP ) == p_vtx ) {
            if( p_bp.m_charT ) {
                return vertex_T( p_bp.m_posT );
            } else {
                return vertex_bot( );
            }
        } else if( vertex_T( p_bp.m_posT ) == p_vtx ) {
            if( p_bp.m_charP ) {
                return vertex_P( p_bp.m_posP );
            } else {
                return vertex_bot( );
            }
        } else if( p_vtx == vertex_bot( ) ) {
            if( p_bp.m_charT ) {
                return vertex_T( p_bp.m_posT );
            } else {
                return vertex_P( p_bp.m_posP );
            }
        }

        return vertex_bot( ) + 1000;
    }

    inference_graph( const std::string& p_P, const std::string& p_T ) : m_P{ p_P }, m_T{ p_T } {
        m_edges = std::vector<std::vector<breakpoint>>{ p_P.size( ) + p_T.size( ) + 1,
                                                        std::vector<breakpoint>{ } };
    }

    void add_alignment( const breakpoint_repn& p_bp ) {
        auto uc = breakpoint_uncompress( p_bp, m_P );
        for( const auto& bp : uc ) {
            if( !bp.m_charT && !bp.m_charP ) { continue; }
            if( !bp.m_charP ) {
                m_edges[ vertex_T( bp.m_posT ) ].push_back( bp );
                m_edges[ vertex_bot( ) ].push_back( bp );
            } else if( !bp.m_charT ) {
                m_edges[ vertex_P( bp.m_posP ) ].push_back( bp );
                m_edges[ vertex_bot( ) ].push_back( bp );
            } else {
                m_edges[ vertex_T( bp.m_posT ) ].push_back( bp );
                m_edges[ vertex_P( bp.m_posP ) ].push_back( bp );
            }
        }
        color( );
    }

    // return connected component as a vector of vtx ids contained
    cc_t component( u64 p_start ) const {
        auto             cl = _color[ p_start ];
        std::vector<u64> cc{ };
        for( u64 i = 0; i < m_edges.size( ); ++i ) {
            if( _color[ i ] == cl ) { cc.push_back( i ); }
        }
        return { _isRed[ cl ], cc };
    }

    // return a vector of all components; component 0 is dummy
    std::vector<cc_t> all_components( ) const {
        std::vector<cc_t> res{ };
        for( bool b : _isRed ) { res.push_back( { b, std::vector<u64>{ } } ); }
        for( u64 i = 0; i < m_edges.size( ); ++i ) { res[ _color[ i ] ].second.push_back( i ); }
        return res;
    }

    std::string black_T( ) const {
        std::string res = "";
        for( u64 i = 0; i < m_T.size( ); ++i ) {
            if( !_isRed[ _color[ vertex_T( i ) ] ] ) { res.push_back( m_T[ i ] ); }
        }
        return res;
    }

    std::string black_P( ) const {
        std::string res = "";
        for( u64 i = 0; i < m_P.size( ); ++i ) {
            if( !_isRed[ _color[ vertex_P( i ) ] ] ) { res.push_back( m_P[ i ] ); }
        }
        return res;
    }

    std::string color_T( ) const {
        std::string res = "";
        for( u64 i = 0; i < m_T.size( ); ++i ) { res.push_back( 'a' + _color[ vertex_T( i ) ] ); }
        return res;
    }

    std::string color_P( ) const {
        std::string res = "";
        for( u64 i = 0; i < m_P.size( ); ++i ) { res.push_back( 'a' + _color[ vertex_P( i ) ] ); }
        return res;
    }
};

// - draw edge between two vertices (either red/black) (should get breakpoint)
//  ~> draw cc
//   ~> draw all cc

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

constexpr auto MAIN_COLOR = COLOR_TEXT;
constexpr auto RED_COLOR  = COLOR_C3;

void place_inference_graph_edge( picture& p_pic, const inference_graph& p_ig, u64 p_start,
                                 u64 p_end, const std::string& p_pname, const std::string& p_tname,
                                 color p_color = MAIN_COLOR, color p_bgColor = COLOR_FILL_WHITE ) {
    if( p_start == p_ig.vertex_bot( ) ) { std::swap( p_start, p_end ); }
    if( p_end == p_ig.vertex_bot( ) ) {
        p_pic.place_vhv_line( p_ig.label_for_vtx( p_start, p_pname, p_tname ),
                              p_ig.mid_label( p_start, p_pname, p_tname ),
                              p_ig.label_for_vtx( p_end, p_pname, p_tname ),
                              OPT::DOUBLE( p_color ) | OPT::DOUBLE_DISTANCE( "0.75pt" )
                                  | OPT::DRAW( p_bgColor ) | OPT::ROUNDED_CORNERS( "3pt" ) );
    } else {
        p_pic.place_line( p_ig.label_for_vtx( p_start, p_pname, p_tname ),
                          p_ig.label_for_vtx( p_end, p_pname, p_tname ),
                          OPT::DOUBLE( p_color ) | OPT::DOUBLE_DISTANCE( "0.75pt" )
                              | OPT::DRAW( p_bgColor ) );
    }
}

std::vector<std::pair<std::string, bool>>
place_inference_graph_coordinates( picture& p_pic, const inference_graph& p_ig,
                                   const std::string& p_pname, const std::string& p_tname,
                                   tikz_point p_PtopLeft, tikz_point p_TtopLeft, u64 p_pShift = 0,
                                   bool p_showStringNames = true ) {
    std::vector<std::pair<std::string, bool>> res{ };

    // place string names
    if( p_showStringNames ) {
        p_pic.place_text( math_mode( VSIZE_CORRECTION + p_pname ),
                          p_PtopLeft + tikz_point{ -1 * CHAR_WIDTH, -.5 * CHAR_HEIGHT } );
        p_pic.place_text( math_mode( VSIZE_CORRECTION + p_tname ),
                          p_TtopLeft + tikz_point{ -1 * CHAR_WIDTH, -.5 * CHAR_HEIGHT } );
    }

    auto pstr = stylized_string{ p_ig.m_P, p_tname,
                                 str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    auto tstr = stylized_string{ p_ig.m_T, p_pname,
                                 str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    // place position number
    // place coordinates
    for( u64 x = 0; x < p_ig.m_P.size( ); ++x ) {
        p_pic.place_text( textsize_footnotesize( std::to_string( x ) ),
                          tikz_point{ p_PtopLeft.m_x + ( 0.5 + x + p_pShift ) * CHAR_WIDTH,
                                      p_PtopLeft.m_y - CHAR_HEIGHT },
                          OPT::INNER_SEP( "2pt" ) | OPT::ANCHOR_NORTH
                              | OPT::TEXT_COLOR( COLOR_TEXT.deemphasize_weak( ) ) );

        tikz_point  pt{ p_PtopLeft.m_x + ( 0.5 + x + p_pShift ) * CHAR_WIDTH,
                        p_PtopLeft.m_y + .875 * CHAR_HEIGHT };
        std::string label = p_ig.label_for_vtx( p_ig.vertex_P( x ), p_pname, p_tname );
        p_pic.place_coordinate( pt, label );
        res.push_back( { label, p_ig.is_red( p_ig.vertex_P( x ) ) } );

        // mark red characters
        if( p_ig.is_red( p_ig.vertex_P( x ) ) ) {
            pstr.annotation_at_pos( x ).m_displayStyle |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
            pstr.annotation_at_pos( x ).m_textColor = RED_COLOR.deemphasize_weak( );
        }
    }
    for( u64 x = 0; x < p_ig.m_T.size( ); ++x ) {
        p_pic.place_text( textsize_footnotesize( std::to_string( x ) ),
                          tikz_point{ p_TtopLeft.m_x + ( 0.5 + x ) * CHAR_WIDTH, p_TtopLeft.m_y },
                          OPT::INNER_SEP( "2pt" ) | OPT::ANCHOR_SOUTH
                              | OPT::TEXT_COLOR( COLOR_TEXT.deemphasize_weak( ) ) );

        tikz_point  pt{ p_TtopLeft.m_x + ( 0.5 + x ) * CHAR_WIDTH,
                        p_TtopLeft.m_y - 1.875 * CHAR_HEIGHT };
        std::string label = p_ig.label_for_vtx( p_ig.vertex_T( x ), p_pname, p_tname );
        p_pic.place_coordinate( pt, label );
        res.push_back( { label, p_ig.is_red( p_ig.vertex_T( x ) ) } );

        // mark red characters
        if( p_ig.is_red( p_ig.vertex_T( x ) ) ) {
            tstr.annotation_at_pos( x ).m_displayStyle |= chr_displ_t::RENDER_AS_FORMER_WILDCARD;
            tstr.annotation_at_pos( x ).m_textColor = RED_COLOR.deemphasize_weak( );
        }
    }

    // place bot vertex
    tikz_point  pt{ p_TtopLeft.m_x + ( .75 + p_ig.m_T.size( ) ) * CHAR_WIDTH,
                    ( p_TtopLeft.m_y + p_PtopLeft.m_y - CHAR_HEIGHT ) / 2.0 };
    std::string label = p_ig.label_for_vtx( p_ig.vertex_bot( ), p_pname, p_tname );
    p_pic.place_coordinate( pt, label );
    res.push_back( { label, true } );

    // place dummy mid coordinates
    tikz_point ptt{ p_TtopLeft.m_x + ( .125 + p_ig.m_T.size( ) ) * CHAR_WIDTH,
                    p_TtopLeft.m_y - 1.375 * CHAR_HEIGHT };
    p_pic.place_coordinate( ptt, p_ig.mid_label( p_ig.vertex_T( 0 ), p_pname, p_tname ) );
    tikz_point ptp{ p_TtopLeft.m_x + ( .125 + p_ig.m_T.size( ) ) * CHAR_WIDTH,
                    p_PtopLeft.m_y + .375 * CHAR_HEIGHT };
    p_pic.place_coordinate( ptp, p_ig.mid_label( p_ig.vertex_P( 0 ), p_pname, p_tname ) );

    // place strings

    place_string( p_pic, tstr, p_TtopLeft );
    place_string( p_pic, pstr, p_PtopLeft + ( p_pShift * CHAR_WIDTH ) );
    if( p_showStringNames ) {
        p_pic.place_text( math_mode( "\\bot" ), pt + ( CHAR_WIDTH / 4.0 ),
                          OPT::INNER_SEP( "0pt" ) | OPT::ANCHOR_WEST );
    }

    return res;
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures; one file per frame
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, {
    constexpr std::string T = "abbbabaabbbababbb";
    constexpr std::string P = "ababbabaabb";

    auto bps    = compute_occs_with_edits( P, T, 3 );
    auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD };
    auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

    breakpoint_repn res{ };
    res.push_back( breakpoint{ 0, 0 } );
    res.push_back( breakpoint{ 2, 2, 'a' } );
    res.push_back( breakpoint{ 9, 8, 0, 'b' } );
    res.push_back( breakpoint{ 11, 11 } );
    bps.push_back( res );

    add_occurrences_pictures( doc, P_NAME, T_NAME, { res, bps[ 4 ] }, occ_style_t::CHAR_POS );

    std::vector<breakpoint_repn> bps2 = { res, bps[ 4 ] };

    bps.clear( );
    {
        breakpoint_repn res{ };
        res.push_back( breakpoint{ 3, 5 } );
        res.push_back( breakpoint{ 4, 6, 'b' } );
        res.push_back( breakpoint{ 6, 7 } );
        bps.push_back( res );
        bps2.push_back( res );
    }
    add_occurrences_pictures( doc, P_NAME.slice( fragmentco{ 3, 6 } ), T_NAME, bps,
                              occ_style_t::CHAR_POS );

    {
        inference_graph ig{ P, T };
        for( const auto& bp : bps2 ) {
            ig.add_alignment( bp );

            WITH_PICTURE( pic, { }, doc ) {
                auto vtc = place_inference_graph_coordinates(
                    pic, ig, "P", "T", { 0, -5 * CHAR_HEIGHT }, { 0, 0 * CHAR_HEIGHT }, 3 );

                for( u64 i = 0; i <= ig.vertex_bot( ); ++i ) {
                    for( const auto& e : ig.m_edges[ i ] ) {
                        auto n = ig.neighbor( i, e );
                        if( n < i ) { continue; }

                        if( e.m_charT != e.m_charP || !e.m_charT || !e.m_charP ) {
                            place_inference_graph_edge( pic, ig, i, n, "P", "T", RED_COLOR.to_bg( ),
                                                        RED_COLOR.deemphasize_strong( ) );
                        } else {
                            place_inference_graph_edge( pic, ig, i, n, "P", "T",
                                                        MAIN_COLOR.deemphasize_strong( ),
                                                        MAIN_COLOR.to_bg( ) );
                        }
                    }
                }

                std::set<u64> nwvtc{ };
                // highlight new alignment
                for( const auto& e : breakpoint_uncompress( bp, P ) ) {
                    if( !e.m_charT && !e.m_charP ) { continue; }

                    u64 st = !e.m_charP ? ig.vertex_bot( ) : ig.vertex_P( e.m_posP );
                    u64 ed = !e.m_charT ? ig.vertex_bot( ) : ig.vertex_T( e.m_posT );
                    nwvtc.insert( st );
                    nwvtc.insert( ed );

                    auto cc = ig.component( st );
                    if( cc.first ) {
                        for( auto vc : cc.second ) { nwvtc.insert( vc ); }
                    }

                    if( e.m_charT != e.m_charP || !e.m_charT || !e.m_charP ) {
                        place_inference_graph_edge( pic, ig, st, ed, "P", "T", RED_COLOR.to_bg( ),
                                                    RED_COLOR );
                    } else {
                        place_inference_graph_edge( pic, ig, st, ed, "P", "T", MAIN_COLOR,
                                                    MAIN_COLOR.to_bg( ) );
                    }
                }

                for( u64 i = 0; i < vtc.size( ); ++i ) {
                    const auto& [ lb, red ] = vtc[ i ];
                    if( nwvtc.count( i ) ) {
                        if( red ) {
                            place_vertex(
                                pic, lb,
                                vertex::marked_vertex(
                                    RED_COLOR, 1.25, .75, vertex::ST_CIRCLE,
                                    vertex::unselected_vertex( COLOR_FILL_WHITE, .75 ) ) );
                        } else {
                            place_vertex( pic, lb,
                                          vertex::marked_vertex(
                                              COLOR_FILL_WHITE, 1.25, .75, vertex::ST_CIRCLE,
                                              vertex::unselected_vertex( MAIN_COLOR, .75 ) ) );
                        }
                    } else {
                        if( red ) {
                            place_vertex(
                                pic, lb,
                                vertex::marked_vertex(
                                    RED_COLOR.deemphasize_strong( ), 1.25, .75, vertex::ST_CIRCLE,
                                    vertex::unselected_vertex( COLOR_FILL_WHITE, .75 ) ) );
                        } else {
                            place_vertex( pic, lb,
                                          vertex::marked_vertex(
                                              COLOR_FILL_WHITE, 1.25, .75, vertex::ST_CIRCLE,
                                              vertex::unselected_vertex(
                                                  MAIN_COLOR.deemphasize_strong( ), .75 ) ) );
                        }
                    }
                }
            }
            // highlight 1 cc
            for( const auto& cc : ig.all_components( ) ) {
                if( cc.second.empty( ) ) { continue; }
                WITH_PICTURE( pic, { }, doc ) {
                    auto vtc = place_inference_graph_coordinates(
                        pic, ig, "P", "T", { 0, -5 * CHAR_HEIGHT }, { 0, 0 * CHAR_HEIGHT }, 3 );

                    // place background
                    for( u64 i = 0; i <= ig.vertex_bot( ); ++i ) {
                        for( const auto& e : ig.m_edges[ i ] ) {
                            auto n = ig.neighbor( i, e );
                            if( n < i ) { continue; }

                            if( e.m_charT != e.m_charP || !e.m_charT || !e.m_charP ) {
                                place_inference_graph_edge( pic, ig, i, n, "P", "T",
                                                            RED_COLOR.to_bg( ),
                                                            RED_COLOR.deemphasize_strong( ) );
                            } else {
                                place_inference_graph_edge( pic, ig, i, n, "P", "T",
                                                            MAIN_COLOR.deemphasize_strong( ),
                                                            MAIN_COLOR.to_bg( ) );
                            }
                        }
                    }

                    std::set<u64> nwvtc{ };
                    // place current cc
                    for( auto i : cc.second ) {
                        nwvtc.insert( i );
                        for( const auto& e : ig.m_edges[ i ] ) {
                            auto n = ig.neighbor( i, e );
                            if( n < i ) { continue; }

                            if( e.m_charT != e.m_charP || !e.m_charT || !e.m_charP ) {
                                place_inference_graph_edge( pic, ig, i, n, "P", "T",
                                                            RED_COLOR.to_bg( ), RED_COLOR );
                            } else {
                                place_inference_graph_edge( pic, ig, i, n, "P", "T", MAIN_COLOR,
                                                            MAIN_COLOR.to_bg( ) );
                            }
                        }
                    }
                    for( u64 i = 0; i < vtc.size( ); ++i ) {
                        const auto& [ lb, red ] = vtc[ i ];
                        if( nwvtc.count( i ) ) {
                            if( red ) {
                                place_vertex(
                                    pic, lb,
                                    vertex::marked_vertex(
                                        RED_COLOR, 1.25, .75, vertex::ST_CIRCLE,
                                        vertex::unselected_vertex( COLOR_FILL_WHITE, .75 ) ) );
                            } else {
                                place_vertex( pic, lb,
                                              vertex::marked_vertex(
                                                  COLOR_FILL_WHITE, 1.25, .75, vertex::ST_CIRCLE,
                                                  vertex::unselected_vertex( MAIN_COLOR, .75 ) ) );
                            }
                        } else {
                            if( red ) {
                                place_vertex(
                                    pic, lb,
                                    vertex::marked_vertex(
                                        RED_COLOR.deemphasize_strong( ), 1.25, .75,
                                        vertex::ST_CIRCLE,
                                        vertex::unselected_vertex( COLOR_FILL_WHITE, .75 ) ) );
                            } else {
                                place_vertex( pic, lb,
                                              vertex::marked_vertex(
                                                  COLOR_FILL_WHITE, 1.25, .75, vertex::ST_CIRCLE,
                                                  vertex::unselected_vertex(
                                                      MAIN_COLOR.deemphasize_strong( ), .75 ) ) );
                            }
                        }
                    }
                }
            }
        }
    }
    /*
        for( u64 y = 0; y < bps.size( ); ++y ) {
            WITH_PICTURE( p3, { }, doc ) {
                place_alignment( p3, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{
       0.0, 1.0 }, bps[ y ], AT_PRINT_EXTRA_STRING_PARTS | AT_COMPRESS );
            }
        }
        */
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
