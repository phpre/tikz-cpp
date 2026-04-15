#include <string>
#include <vector>

#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font_default";
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
                if( !_color[ i ] ) { color( i, _isRed.size( ) ); }
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

    inline u64 neighbor( u64 p_vtx, breakpoint p_bp ) const {
        if( p_bp.m_posP == vertex_P( p_vtx ) ) {
            if( p_bp.m_charT ) {
                return vertex_T( p_bp.m_posT );
            } else {
                return vertex_bot( );
            }
        } else if( p_bp.m_posT == vertex_T( p_vtx ) ) {
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

        return vertex_bot( ) + 1;
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

// - draw basic graph, return labels for vertex positions
// - draw edge between two vertices (either red/black) (should get breakpoint)
//  ~> draw cc
//   ~> draw all cc

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

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
        inference_graph i_g{ P, T };
        for( const auto& bp : bps2 ) {
            i_g.add_alignment( bp );

            printf( "P %s %s\nT %s %s\n\n", i_g.color_P( ).c_str( ), i_g.black_P( ).c_str( ),
                    i_g.color_T( ).c_str( ), i_g.black_T( ).c_str( ) );

            // draw vertices
            // draw edges
            // highlight 1 cc
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
